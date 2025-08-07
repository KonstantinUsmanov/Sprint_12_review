class PersonLoader {
public:
    struct Config {
        string db_name;
        int db_connection_timeout = 0;
        bool db_allow_exceptions = false;
        DBLogLevel db_log_level = DBLogLevel::None;
    };

    PersonLoader(Config config, unique_ptr<IDBConnector> connector = nullptr)
        : config_(move(config)), connector_(connector ?: make_unique<DBConnector>()) {}

    vector<Person> Load(int min_age, int max_age, string_view name_filter) {
        auto db = Connect();
        if (!db.IsOK()) return {};

        auto query = BuildQuery(min_age, max_age, name_filter);
        return LoadPersons(db, query);
    }

private:
    DBHandler Connect() const {
        if (config_.db_name.starts_with("tmp."s)) {
            return connector_->ConnectTmp(config_.db_name, config_.db_connection_timeout);
        }
        return connector_->Connect(config_.db_name, config_.db_connection_timeout);
    }

    DBQuery BuildQuery(int min_age, int max_age, string_view name_filter) const {
        return DBQuery(fmt::format(
            "from Persons select Name, Age where Age between {} and {} and Name like '%{}%'",
            min_age, max_age, connector_->Quote(name_filter)));
    }

    vector<Person> LoadPersons(DBHandler& db, const DBQuery& query) const {
        vector<Person> persons;
        for (auto [name, age] : db.LoadRows<string, int>(query)) {
            persons.push_back({move(name), age});
        }
        return persons;
    }

    Config config_;
    unique_ptr<IDBConnector> connector_;
};
