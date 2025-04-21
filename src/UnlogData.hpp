#include <Geode/Loader.hpp>

struct UnlogData {
    typedef std::unordered_map<std::string, UnlogData> Map;

    inline static Map data;

    UnlogData() : debug(true), info(true), warn(true), error(true) {}
    UnlogData(bool value) : debug(value), info(value), warn(value), error(value) {}
    UnlogData(bool debug, bool info, bool warn, bool error) : debug(debug), info(info), warn(warn), error(error) {}
    UnlogData(const UnlogData&) = default;

    bool& operator[](int severity) {
        switch (severity) {
            case geode::Severity::Debug: return debug;
            case geode::Severity::Info: return info;
            case geode::Severity::Warning: return warn;
            case geode::Severity::Error: return error;
            default: return debug;
        }
    }

    bool debug;
    bool info;
    bool warn;
    bool error;
};

template<>
struct matjson::Serialize<UnlogData::Map> {
    static geode::Result<UnlogData::Map> fromJson(const matjson::Value& value) {
        if (!value.isObject()) {
            return geode::Err("Invalid JSON format, expected an object");
        }

        UnlogData::Map map;
        for (const auto& [key, val] : value) {
            if (!val.isObject()) {
                return geode::Err(fmt::format("Invalid JSON format for key '{}', expected an object", key));
            }

            map[key] = {
                value["debug"].asBool().unwrapOr(true),
                value["info"].asBool().unwrapOr(true),
                value["warn"].asBool().unwrapOr(true),
                value["error"].asBool().unwrapOr(true)
            };
        }

        return geode::Ok(map);
    }

    static matjson::Value toJson(const UnlogData::Map& data) {
        matjson::Value map;
        for (const auto& [key, val] : data) {
            map[key] = matjson::makeObject({
                { "debug", val.debug },
                { "info", val.info },
                { "warn", val.warn },
                { "error", val.error }
            });
        }
        return map;
    }
};
