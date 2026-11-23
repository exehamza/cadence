/****************************************************************************
** Meta object code from reading C++ file 'AnalyticsViewModel.hpp'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/viewmodels/AnalyticsViewModel.hpp"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AnalyticsViewModel.hpp' doesn't include <QObject>."
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
struct qt_meta_tag_ZN7cadence18AnalyticsViewModelE_t {};
} // unnamed namespace

template <> constexpr inline auto cadence::AnalyticsViewModel::qt_create_metaobjectdata<qt_meta_tag_ZN7cadence18AnalyticsViewModelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "cadence::AnalyticsViewModel",
        "timeframeChanged",
        "",
        "analyticsChanged",
        "countdownChanged",
        "updateExamCountdowns",
        "refresh",
        "timeframeIndex",
        "totalHoursInRange",
        "activeDaysLastYear",
        "currentStreakDays",
        "longestStreakDays",
        "subjectDistribution",
        "QVariantList",
        "heatmapCells",
        "heatmapMonthLabels",
        "upcomingExams"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'timeframeChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'analyticsChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'countdownChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'updateExamCountdowns'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Method 'refresh'
        QtMocHelpers::MethodData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'timeframeIndex'
        QtMocHelpers::PropertyData<int>(7, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'totalHoursInRange'
        QtMocHelpers::PropertyData<double>(8, QMetaType::Double, QMC::DefaultPropertyFlags, 1),
        // property 'activeDaysLastYear'
        QtMocHelpers::PropertyData<int>(9, QMetaType::Int, QMC::DefaultPropertyFlags, 1),
        // property 'currentStreakDays'
        QtMocHelpers::PropertyData<int>(10, QMetaType::Int, QMC::DefaultPropertyFlags, 1),
        // property 'longestStreakDays'
        QtMocHelpers::PropertyData<int>(11, QMetaType::Int, QMC::DefaultPropertyFlags, 1),
        // property 'subjectDistribution'
        QtMocHelpers::PropertyData<QVariantList>(12, 0x80000000 | 13, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 1),
        // property 'heatmapCells'
        QtMocHelpers::PropertyData<QVariantList>(14, 0x80000000 | 13, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 1),
        // property 'heatmapMonthLabels'
        QtMocHelpers::PropertyData<QVariantList>(15, 0x80000000 | 13, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 1),
        // property 'upcomingExams'
        QtMocHelpers::PropertyData<QVariantList>(16, 0x80000000 | 13, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 2),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<AnalyticsViewModel, qt_meta_tag_ZN7cadence18AnalyticsViewModelE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject cadence::AnalyticsViewModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7cadence18AnalyticsViewModelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7cadence18AnalyticsViewModelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7cadence18AnalyticsViewModelE_t>.metaTypes,
    nullptr
} };

void cadence::AnalyticsViewModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<AnalyticsViewModel *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->timeframeChanged(); break;
        case 1: _t->analyticsChanged(); break;
        case 2: _t->countdownChanged(); break;
        case 3: _t->updateExamCountdowns(); break;
        case 4: _t->refresh(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (AnalyticsViewModel::*)()>(_a, &AnalyticsViewModel::timeframeChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (AnalyticsViewModel::*)()>(_a, &AnalyticsViewModel::analyticsChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (AnalyticsViewModel::*)()>(_a, &AnalyticsViewModel::countdownChanged, 2))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<int*>(_v) = _t->timeframeIndex(); break;
        case 1: *reinterpret_cast<double*>(_v) = _t->totalHoursInRange(); break;
        case 2: *reinterpret_cast<int*>(_v) = _t->activeDaysLastYear(); break;
        case 3: *reinterpret_cast<int*>(_v) = _t->currentStreakDays(); break;
        case 4: *reinterpret_cast<int*>(_v) = _t->longestStreakDays(); break;
        case 5: *reinterpret_cast<QVariantList*>(_v) = _t->subjectDistribution(); break;
        case 6: *reinterpret_cast<QVariantList*>(_v) = _t->heatmapCells(); break;
        case 7: *reinterpret_cast<QVariantList*>(_v) = _t->heatmapMonthLabels(); break;
        case 8: *reinterpret_cast<QVariantList*>(_v) = _t->upcomingExams(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setTimeframeIndex(*reinterpret_cast<int*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *cadence::AnalyticsViewModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *cadence::AnalyticsViewModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7cadence18AnalyticsViewModelE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int cadence::AnalyticsViewModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void cadence::AnalyticsViewModel::timeframeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void cadence::AnalyticsViewModel::analyticsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void cadence::AnalyticsViewModel::countdownChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
