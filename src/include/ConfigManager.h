// $Id: ConfigManager.h 40 2010-07-16 22:04:49Z mgpensar $

#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#ifdef VISUAL_STUDIO
#include "stdafx.h"
#endif
#include <cstddef> // NULL
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include <boost/archive/tmpdir.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/hash_map.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/assume_abstract.hpp>
#include <boost/archive/xml_oarchive.hpp> // Archive for writing XML
#include <boost/archive/xml_iarchive.hpp> // Archive for reading XML
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include <boost/filesystem/fstream.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>

namespace fs = boost::filesystem;

namespace pensar_digital
{
    namespace cpplib
    {
        template <typename StringType = std::string, typename OutputArchiveType = boost::archive::text_oarchive, typename InputArchiveType = boost::archive::text_iarchive, typename SerializableKeyType = StringType, typename SerializableValueType = StringType>
        class ConfigManager
        {
            public:

            /// Property
            class Property
            {
                public:
                SerializableKeyType key;
                SerializableValueType value;
                StringType display_name;

                // Added this default constructor just for serialization. Do not use it.
                Property () {};
                Property (SerializableKeyType key_, const SerializableValueType& value_, StringType display_name_ = ""): key(key_), value(value_), display_name(display_name_) {}

                template<class Archive>
                void serialize(Archive & ar, const unsigned int version)
                {
                    ar & BOOST_SERIALIZATION_NVP(key);
                    ar & BOOST_SERIALIZATION_NVP(value);
                    ar & BOOST_SERIALIZATION_NVP(display_name);
                }
            };

            typedef boost::shared_ptr<Property> PropertyPtr;
            typedef std::map<SerializableKeyType, PropertyPtr > PropertiesMap;

            private:
            fs::path full_path;
            PropertiesMap cfg;

            public:
            ConfigManager<StringType, OutputArchiveType, InputArchiveType, SerializableKeyType, SerializableValueType> (fs::path fullpath = "config.txt") : full_path (fullpath)
            {
                full_path = fs::system_complete (fullpath);
                load ();
            };

            void save ()
            {
                fs::ofstream ofs (full_path);
                OutputArchiveType oa (ofs);
                oa << cfg;
            }

            void load ()
            {
                if (fs::exists (full_path))
                {
                    fs::ifstream ifs (full_path);
                    InputArchiveType ia (ifs);
                    ia >> cfg;
                }
            }

            template <typename T>
            T get (const SerializableKeyType& key)
            {
                return boost::lexical_cast<T>(cfg[key]->value);
            }

            void set (const SerializableKeyType& key, const SerializableValueType& value, StringType display_name = "")
            {
                PropertyPtr pp (new Property (key, value, display_name));
                cfg[key] = pp;
            }
        };
    }   // namespace cpplib
}       // namespace pensar_digital


#endif
