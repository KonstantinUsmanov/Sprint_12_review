namespace {
void ValidateYear(int year) {
    if (year < 1 || year > 9999) {
        throw domain_error("year is "s + (year < 1 ? "too small" : "too big"));
    }
}

void ValidateMonth(int month) {
    if (month < 1 || month > 12) {
        throw domain_error("month is "s + (month < 1 ? "too small" : "too big"));
    }
}

void ValidateDay(int day, int month, int year) {
    const bool is_leap_year = (year % 4 == 0) && !(year % 100 == 0 && year % 400 != 0);
    const array month_lengths = {31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (day < 1 || day > month_lengths[month - 1]) {
        throw domain_error("day is "s + (day < 1 ? "too small" : "too big"));
    }
}

// МОЖНО ЛУЧШЕ
// Можно написать ещё одну функцию ValidateTime, в которой вызвать функции
// в строках 37, 38, 39. Так главная функция станет короче.
void ValidateTimeComponent(int value, int max, const string& component) {
    if (value < 0 || value > max) {
        throw domain_error(component + " is "s + (value < 0 ? "too small" : "too big"));
    }
}
}

void CheckDateTimeValidity(const DateTime& dt) {
    ValidateYear(dt.year);
    ValidateMonth(dt.month);
    ValidateDay(dt.day, dt.month, dt.year);
    ValidateTimeComponent(dt.hour, 23, "hour");
    ValidateTimeComponent(dt.minute, 59, "minute");
    ValidateTimeComponent(dt.second, 59, "second");
}
