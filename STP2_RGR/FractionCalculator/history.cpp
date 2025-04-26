#include "history.h"

History::History() {
}

void History::addRecord(const QString& expressionResult) {
    if (records.size() >= maxHistorySize) {
        records.removeFirst();
    }
    records.append(CalcRecord(expressionResult));
}

CalcRecord History::record(int index) const {
    if (index >= 0 && index < records.size()) {
        return records.at(index);
    }
    return CalcRecord("");
}

void History::clear() {
    records.clear();
}

int History::count() const {
    return records.size();
}

const QList<CalcRecord>& History::getRecords() const {
    return records;
}
