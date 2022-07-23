#pragma once

#include <string>
#include <map>
#include <type_traits>
#include <variant>
#include <optional>

#include "core/Log.h"

namespace Anwill {

    /**
     * bool: conditional type, values may be either true or false.
     * int: a signed, two's complement, 32-bit integer.
     * uint: an unsigned 32-bit integer.
     * float: an IEEE-754 single-precision floating point number.
     * double: an IEEE-754 double-precision floating-point number.
     */
    class ShaderMacros {
    public:
        typedef std::variant<bool, int, float, double> shaderMacroTypes;

        template <typename T>
        static std::optional<T> GetMacro(const std::string& identifier)
        {
            if (s_Map.contains(identifier)) {
                auto variant = s_Map[identifier];
                switch (variant.index())
                {
                    case 0:
                        return (T) std::get<bool>(variant);
                    case 1:
                        return (T) std::get<int>(variant);
                    case 2:
                        return (T) std::get<float>(variant);
                    case 3:
                        return (T) std::get<double>(variant);
                    default:
                        AW_FATAL("ShaderMacro variant index not recognized!");
                        return {};
                }
            }
            AW_ERROR("The following ShaderMacro could not be found: {0}", identifier);
            return {};
        }

        template <typename T>
        static void SetMacro(const std::string& identifier, T t) {
            s_Map[identifier] = t;
        }

        static void ReplaceAllIdentifiers(std::string& shaderSrc) {
            for (auto const& [key, val] : s_Map) {
                // Index has to match shaderMacroTypes
                switch(val.index()) {
                    case 0:
                        ReplaceIdentifier(shaderSrc, key,
                                          GetValueFromVariantIndex<bool>(val));
                        break;
                    case 1:
                        ReplaceIdentifier(shaderSrc, key,
                                          GetValueFromVariantIndex<int>(val));
                        break;
                    case 2:
                        ReplaceIdentifier(shaderSrc, key,
                                          GetValueFromVariantIndex<float>(val));
                        break;
                    case 3:
                        ReplaceIdentifier(shaderSrc, key,
                                          GetValueFromVariantIndex<double>(val));
                        break;
                    default:
                        AW_FATAL("ShaderMacro variant index not recognized!");
                        break;
                }
            }
        }

    private:
        inline static std::map<std::string, shaderMacroTypes> s_Map;

        template <typename T>
        static void ReplaceIdentifier(std::string& shaderSrc,
                                      const std::string& identifier,
                                      T value)
        {
            std::size_t pos = shaderSrc.find(identifier);
            if(pos != std::string::npos) {
                shaderSrc.erase(pos, identifier.size());
                shaderSrc.insert(pos, ShaderTypeToString(value));
            }
        }

        template <typename T>
        static T GetValueFromVariantIndex(const shaderMacroTypes& value) {
                return std::get<T>(value);
        }

        template <typename T>
        static std::string ShaderTypeToString(T value) {
            if (std::is_same<T, bool>()) {
                bool b = value; // Avoid compiler warning *shrug*
                return b ? "true" : "false";
                // std::to_string() returns 1 and 0 ...
            } else {
                return std::to_string(value);
            }
        }
    };
}

