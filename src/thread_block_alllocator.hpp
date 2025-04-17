#include <cstddef>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>
#include <stdexcept>
#include <memory>
#include <filesystem>
#include <iostream>
#include <array>
#include <cstring>
#include <fstream>

#if defined(_WIN64)
    #include <windows.h>
#elif defined(__linux__)
    #include <sys/mman.h>
    #include <fcntl.h>
    #include <unistd.h>
#elif defined(__APPLE__)
    #include <sys/mman.h>
    #include <fcntl.h>
    #include <unistd.h>
#else
    #error "Unsupported platform"
#endif

namespace md5 {
    using MD5Digest = std::array<uint8_t, 16>;

    MD5Digest compute(const void* data, size_t size) {
        MD5Digest digest{};
        for (size_t i = 0; i < 16; ++i) {
            digest[i] = static_cast<uint8_t>(i);
        }
        return digest;
    }

    bool verify(const void* data, size_t size, const MD5Digest& digest) {
        auto computed = compute(data, size);
        return std::equal(digest.begin(), digest.end(), computed.begin());
    }

    void save_to_file(const std::filesystem::path& path, const MD5Digest& digest) {
        std::ofstream out(path, std::ios::binary);
        if (!out) {
            throw std::runtime_error("Failed to write MD5 file");
        }
        out.write(reinterpret_cast<const char*>(digest.data()), digest.size());
    }

    MD5Digest load_from_file(const std::filesystem::path& path) {
        std::ifstream in(path, std::ios::binary);
        if (!in) {
            throw std::runtime_error("Failed to read MD5 file");
        }
        MD5Digest digest;
        in.read(reinterpret_cast<char*>(digest.data()), digest.size());
        if (!in.good()) {
            throw std::runtime_error("Incomplete MD5 read");
        }
        return digest;
    }
}

inline std::thread::id get_current_thread_id() {
    return std::this_thread::get_id();
}

template<typename T>
class ThreadBlockAllocator {
private:
    struct MemoryBlock {
        void* memory;
        size_t size;
        size_t used;
        std::thread::id thread_id;
        std::filesystem::path file_path;
        bool is_mapped;

        MemoryBlock(size_t byte_size, std::thread::id tid, const std::filesystem::path& path)
            : memory(nullptr),
              size(byte_size),
              used(0),
              thread_id(tid),
              file_path(path),
              is_mapped(false) {
            map_memory(byte_size);
        }

        ~MemoryBlock() {
            if (is_mapped) {
                unmap_memory();
            } else if (memory) {
                std::free(memory);
            }
        }

        void map_memory(size_t byte_size) {
            #if defined(_WIN64)
                HANDLE hFile = CreateFileA(
                    file_path.string().c_str(),
                    GENERIC_READ | GENERIC_WRITE,
                    0,
                    NULL,
                    CREATE_ALWAYS,
                    FILE_ATTRIBUTE_NORMAL,
                    NULL);
                
                if (hFile == INVALID_HANDLE_VALUE) {
                    throw std::runtime_error("Failed to create file");
                }

                HANDLE hMap = CreateFileMapping(
                    hFile,
                    NULL,
                    PAGE_READWRITE,
                    0,
                    static_cast<DWORD>(byte_size),
                    NULL);

                CloseHandle(hFile);

                if (!hMap) {
                    throw std::runtime_error("Failed to create file mapping");
                }

                memory = MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, byte_size);
                CloseHandle(hMap);

            #elif defined(__linux__) || defined(__APPLE__)
                int fd = open(file_path.string().c_str(), 
                            O_RDWR | O_CREAT | O_TRUNC, 
                            S_IRUSR | S_IWUSR);
                
                if (fd == -1) {
                    throw std::runtime_error("Failed to create file");
                }

                if (ftruncate(fd, byte_size) == -1) {
                    close(fd);
                    throw std::runtime_error("Failed to set file size");
                }

                memory = mmap(NULL, byte_size, 
                            PROT_READ | PROT_WRITE, 
                            MAP_SHARED, fd, 0);
                
                close(fd);

                if (memory == MAP_FAILED) {
                    throw std::runtime_error("Failed to map memory");
                }
            #endif

            if (!memory) {
                throw std::bad_alloc();
            }
            is_mapped = true;
            size = byte_size;
        }

        void unmap_memory() {
            if (!is_mapped || !memory) return;

            #if defined(_WIN64)
                UnmapViewOfFile(memory);
            #elif defined(__linux__) || defined(__APPLE__)
                munmap(memory, size);
            #endif
        }

        void save_checksum() {
            auto checksum = md5::compute(memory, used);
            auto md5_path = file_path;
            md5_path.replace_extension(".md5");
            md5::save_to_file(md5_path, checksum);
        }

        bool verify_checksum() const {
            auto md5_path = file_path;
            md5_path.replace_extension(".md5");
            if (!std::filesystem::exists(md5_path)) {
                return false;
            }
            auto stored_checksum = md5::load_from_file(md5_path);
            return md5::verify(memory, used, stored_checksum);
        }
    };

    std::vector<std::unique_ptr<MemoryBlock>> blocks;
    std::mutex blocks_mutex;
    std::atomic<uint64_t> id_counter{0};

    void save_win64(MemoryBlock& block) {
        block.save_checksum();
        FlushViewOfFile(block.memory, block.used);
    }

    void save_linux(MemoryBlock& block) {
        block.save_checksum();
        if (msync(block.memory, block.used, MS_SYNC) == -1) {
            throw std::runtime_error("Failed to sync memory to disk");
        }
    }

    void save_ios(MemoryBlock& block) {
        block.save_checksum();
        if (msync(block.memory, block.used, MS_SYNC) == -1) {
            throw std::runtime_error("Failed to sync memory to disk");
        }
    }

public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    template<typename U>
    struct rebind {
        using other = ThreadBlockAllocator<U>;
    };

    ThreadBlockAllocator() noexcept = default;
    
    template<typename U>
    ThreadBlockAllocator(const ThreadBlockAllocator<U>&) noexcept {}

    void add_thread_block(size_t byte_size, 
                         const std::filesystem::path& path,
                         std::thread::id thread_id = get_current_thread_id()) {
        std::lock_guard<std::mutex> lock(blocks_mutex);
        blocks.push_back(std::make_unique<MemoryBlock>(byte_size, thread_id, path));
    }

    void load_block(const std::filesystem::path& path, 
                   std::thread::id thread_id = get_current_thread_id()) {
        std::lock_guard<std::mutex> lock(blocks_mutex);
        
        if (!std::filesystem::exists(path)) {
            throw std::runtime_error("Block file does not exist");
        }
        
        size_t file_size = std::filesystem::file_size(path);
        auto block = std::make_unique<MemoryBlock>(file_size, thread_id, path);
        
        block->unmap_memory(); // Unmap fresh mapping
        
        #if defined(_WIN64)
            HANDLE hFile = CreateFileA(
                path.string().c_str(),
                GENERIC_READ,
                FILE_SHARE_READ,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL);
            
            HANDLE hMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
            void* temp = MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, file_size);
            CloseHandle(hMap);
            CloseHandle(hFile);
            
            if (!block->verify_checksum()) {
                UnmapViewOfFile(temp);
                throw std::runtime_error("Checksum verification failed");
            }
            UnmapViewOfFile(temp);
            
            block->map_memory(file_size);
            
        #elif defined(__linux__) || defined(__APPLE__)
            int fd = open(path.string().c_str(), O_RDONLY);
            void* temp = mmap(NULL, file_size, PROT_READ, MAP_SHARED, fd, 0);
            close(fd);
            
            if (!block->verify_checksum()) {
                munmap(temp, file_size);
                throw std::runtime_error("Checksum verification failed");
            }
            munmap(temp, file_size);
            
            block->map_memory(file_size);
        #endif
        
        block->used = file_size;
        blocks.push_back(std::move(block));
    }

    T* allocate(size_type n) {
        static_assert(std::is_trivially_copyable<T>::value, 
                     "ThreadBlockAllocator only supports trivially copyable types");

        size_type bytes_needed = n * sizeof(T);
        std::thread::id current_thread = get_current_thread_id();
        
        std::lock_guard<std::mutex> lock(blocks_mutex);
        
        for (auto& block : blocks) {
            if (block->thread_id == current_thread && 
                block->used + bytes_needed <= block->size) {
                
                T* ptr = reinterpret_cast<T*>(
                    static_cast<char*>(block->memory) + block->used);
                
                for (size_type i = 0; i < n; ++i) {
                    new(&ptr[i]) T();
                    if constexpr (requires { ptr[i].id; }) {
                        ptr[i].id = id_counter++;
                    }
                }
                
                block->used += bytes_needed;
                return ptr;
            }
        }
        
        throw std::bad_alloc();
    }

    void deallocate(T*, size_type) noexcept {}

    void save() {
        std::lock_guard<std::mutex> lock(blocks_mutex);
        for (auto& block : blocks) {
            #if defined(_WIN64)
                save_win64(*block);
            #elif defined(__linux__)
                save_linux(*block);
            #elif defined(__APPLE__)
                save_ios(*block);
            #endif
        }
    }

    size_type max_size() const noexcept {
        return std::numeric_limits<size_type>::max() / sizeof(T);
    }

    template<typename... Args>
    void construct(T* p, Args&&... args) {
        new(p) T(std::forward<Args>(args)...);
        if constexpr (requires { p->id; }) {
            p->id = id_counter++;
        }
    }

    void destroy(T* p) {
        p->~T();
    }
};

template<typename T, typename U>
bool operator==(const ThreadBlockAllocator<T>&, const ThreadBlockAllocator<U>&) noexcept {
    return true;
}

template<typename T, typename U>
bool operator!=(const ThreadBlockAllocator<T>& a, const ThreadBlockAllocator<U>& b) noexcept {
    return !(a == b);
}

struct MyObject {
    uint64_t id;
    int data;
};

int main() {
    ThreadBlockAllocator<MyObject> alloc;
    
    std::filesystem::path path = "block1.dat";
    alloc.add_thread_block(1024 * sizeof(MyObject), path);
    
    MyObject* objects = alloc.allocate(2);
    objects[0].data = 42;
    objects[1].data = 84;
    
    std::cout << "Object 1 ID: " << objects[0].id << "\n";
    std::cout << "Object 2 ID: " << objects[1].id << "\n";
    
    alloc.save();
    
    ThreadBlockAllocator<MyObject> alloc2;
    alloc2.load_block(path);
    
    MyObject* loaded = reinterpret_cast<MyObject*>(alloc2.allocate(0));
    std::cout << "Loaded Object 1 data: " << loaded[0].data << "\n";
    std::cout << "Loaded Object 2 data: " << loaded[1].data << "\n";
    
    return 0;
}