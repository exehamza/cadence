/****************************************************************************
** Meta object code from reading C++ file 'CalendarViewModel.hpp'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/viewmodels/CalendarViewModel.hpp"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CalendarViewModel.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN7cadence17CalendarViewModelE_t {};
} // unnamed namespace

template <> constexpr inline auto cadence::CalendarViewModel::qt_create_metaobjectdata<qt_meta_tag_ZN7cadence17CalendarViewModelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "cadence::CalendarViewModel",
        "viewModeChanged",
        "",
        "selectedDateChanged",
        "gridChanged",
        "refresh",
        "nextPeriod",
        "previousPeriod",
        "goToToday",
        "addStandaloneEvent",
        "title",
        "location",
        "colorHex",
        "dateIso",
        "startTimeStr",
        "endTimeStr",
        "isAllDay",
        "subjectId",
        "removeStandaloneEvent",
        "eventId",
        "viewMode",
        "ViewMode",
        "headerTitle",
        "selectedDateIso",
        "selectedDateFormatted",
        "dayCells",
        "QVariantList",
        "selectedDayItems",
        "MonthView",
        "WeekView"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'viewModeChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'selectedDateChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'gridChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'refresh'
        QtMocHelpers::MethodData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'nextPeriod'
        QtMocHelpers::MethodData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'previousPeriod'
        QtMocHelpers::MethodData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'goToToday'
        QtMocHelpers::MethodData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'addStandaloneEvent'
        QtMocHelpers::MethodData<bool(const QString &, const QString &, const QString &, const QString &, const QString &, const QString &, bool, qint64)>(9, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 10 }, { QMetaType::QString, 11 }, { QMetaType::QString, 12 }, { QMetaType::QString, 13 },
            { QMetaType::QString, 14 }, { QMetaType::QString, 15 }, { QMetaType::Bool, 16 }, { QMetaType::LongLong, 17 },
        }}),
        // Method 'removeStandaloneEvent'
        QtMocHelpers::MethodData<bool(qint64)>(18, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::LongLong, 19 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'viewMode'
        QtMocHelpers::PropertyData<enum ViewMode>(20, 0x80000000 | 21, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet, 0),
        // property 'headerTitle'
        QtMocHelpers::PropertyData<QString>(22, QMetaType::QString, QMC::DefaultPropertyFlags, 2),
        // property 'selectedDateIso'
        QtMocHelpers::PropertyData<QString>(23, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 1),
        // property 'selectedDateFormatted'
        QtMocHelpers::PropertyData<QString>(24, QMetaType::QString, QMC::DefaultPropertyFlags, 1),
        // property 'dayCells'
        QtMocHelpers::PropertyData<QVariantList>(25, 0x80000000 | 26, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 2),
        // property 'selectedDayItems'
        QtMocHelpers::PropertyData<QVariantList>(27, 0x80000000 | 26, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 2),
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'ViewMode'
        QtMocHelpers::EnumData<enum ViewMode>(21, 21, QMC::EnumFlags{}).add({
            {   28, ViewMode::MonthView },
            {   29, ViewMode::WeekView },
        }),
    };
    return QtMocHelpers::metaObjectData<CalendarViewModel, qt_meta_tag_ZN7cadence17CalendarViewModelE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject cadence::CalendarViewModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7cadence17CalendarViewModelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7cadence17CalendarViewModelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7cadence17CalendarViewModelE_t>.metaTypes,
    nullptr
} };

void cadence::CalendarViewModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<CalendarViewModel *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->viewModeChanged(); break;
        case 1: _t->selectedDateChanged(); break;
        case 2: _t->gridChanged(); break;
        case 3: _t->refresh(); break;
        case 4: _t->nextPeriod(); break;
        case 5: _t->previousPeriod(); break;
        case 6: _t->goToToday(); break;
        case 7: { bool _r = _t->addStandaloneEvent((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[5])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[6])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[7])),(*reinterpret_cast<std::add_pointer_t<qint64>>(_a[8])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 8: { bool _r = _t->removeStandaloneEvent((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (CalendarViewModel::*)()>(_a, &CalendarViewModel::viewModeChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (CalendarViewModel::*)()>(_a, &CalendarViewModel::selectedDateChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (CalendarViewModel::*)()>(_a, &CalendarViewModel::gridChanged, 2))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<enum ViewMode*>(_v) = _t->viewMode(); break;
        case 1: *reinterpret_cast<QString*>(_v) = _t->headerTitle(); break;
        case 2: *reinterpret_cast<QString*>(_v) = _t->selectedDateIso(); break;
        case 3: *reinterpret_cast<QString*>(_v) = _t->selectedDateFormatted(); break;
        case 4: *reinterpret_cast<QVariantList*>(_v) = _t->dayCells(); break;
        case 5: *reinterpret_cast<QVariantList*>(_v) = _t->selectedDayItems(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setViewMode(*reinterpret_cast<enum ViewMode*>(_v)); break;
        case 2: _t->setSelectedDateIso(*reinterpret_cast<QString*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *cadence::CalendarViewModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *cadence::CalendarViewModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7cadence17CalendarViewModelE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int cadence::CalendarViewModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void cadence::CalendarViewModel::viewModeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void cadence::CalendarViewModel::selectedDateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void cadence::CalendarViewModel::gridChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
