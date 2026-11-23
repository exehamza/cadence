/****************************************************************************
** Meta object code from reading C++ file 'FocusTimerViewModel.hpp'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/viewmodels/FocusTimerViewModel.hpp"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FocusTimerViewModel.hpp' doesn't include <QObject>."
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
struct qt_meta_tag_ZN7cadence19FocusTimerViewModelE_t {};
} // unnamed namespace

template <> constexpr inline auto cadence::FocusTimerViewModel::qt_create_metaobjectdata<qt_meta_tag_ZN7cadence19FocusTimerViewModelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "cadence::FocusTimerViewModel",
        "modeChanged",
        "",
        "phaseChanged",
        "runningChanged",
        "timeChanged",
        "configChanged",
        "breakOfferChanged",
        "activeSubjectChanged",
        "mediaSettingsChanged",
        "audioPlaybackStateChanged",
        "focusSessionLogged",
        "subjectName",
        "minutesLogged",
        "breakFinished",
        "onTick",
        "start",
        "pause",
        "toggleStartPause",
        "reset",
        "completeAndLogSession",
        "acceptFlowtimeBreak",
        "dismissFlowtimeBreak",
        "skipBreak",
        "selectSubject",
        "subjectId",
        "name",
        "colorHex",
        "clearBackgroundMedia",
        "clearAudioTrack",
        "mode",
        "TimerMode",
        "phase",
        "TimerPhase",
        "isRunning",
        "isPaused",
        "elapsedSeconds",
        "remainingSeconds",
        "formattedTime",
        "progress",
        "pomodoroFocusMinutes",
        "pomodoroBreakMinutes",
        "flowtimeBreakDivisor",
        "hasPendingBreakOffer",
        "suggestedBreakMinutes",
        "activeSubjectId",
        "activeSubjectName",
        "activeSubjectColor",
        "backgroundType",
        "BackgroundType",
        "backgroundColor",
        "backgroundMediaUrl",
        "QUrl",
        "audioTrackUrl",
        "audioTrackName",
        "audioVolume",
        "audioMuted",
        "shouldPlayFocusAudio",
        "Pomodoro",
        "Flowtime",
        "Focus",
        "Break",
        "SolidColor",
        "Image",
        "Video"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'modeChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'phaseChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'runningChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'timeChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'configChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'breakOfferChanged'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'activeSubjectChanged'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'mediaSettingsChanged'
        QtMocHelpers::SignalData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'audioPlaybackStateChanged'
        QtMocHelpers::SignalData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'focusSessionLogged'
        QtMocHelpers::SignalData<void(const QString &, int)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 12 }, { QMetaType::Int, 13 },
        }}),
        // Signal 'breakFinished'
        QtMocHelpers::SignalData<void()>(14, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onTick'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Method 'start'
        QtMocHelpers::MethodData<void()>(16, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'pause'
        QtMocHelpers::MethodData<void()>(17, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'toggleStartPause'
        QtMocHelpers::MethodData<void()>(18, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'reset'
        QtMocHelpers::MethodData<void()>(19, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'completeAndLogSession'
        QtMocHelpers::MethodData<void()>(20, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'acceptFlowtimeBreak'
        QtMocHelpers::MethodData<void()>(21, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'dismissFlowtimeBreak'
        QtMocHelpers::MethodData<void()>(22, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'skipBreak'
        QtMocHelpers::MethodData<void()>(23, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'selectSubject'
        QtMocHelpers::MethodData<void(qint64, const QString &, const QString &)>(24, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::LongLong, 25 }, { QMetaType::QString, 26 }, { QMetaType::QString, 27 },
        }}),
        // Method 'clearBackgroundMedia'
        QtMocHelpers::MethodData<void()>(28, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'clearAudioTrack'
        QtMocHelpers::MethodData<void()>(29, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'mode'
        QtMocHelpers::PropertyData<enum TimerMode>(30, 0x80000000 | 31, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet, 0),
        // property 'phase'
        QtMocHelpers::PropertyData<enum TimerPhase>(32, 0x80000000 | 33, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 1),
        // property 'isRunning'
        QtMocHelpers::PropertyData<bool>(34, QMetaType::Bool, QMC::DefaultPropertyFlags, 2),
        // property 'isPaused'
        QtMocHelpers::PropertyData<bool>(35, QMetaType::Bool, QMC::DefaultPropertyFlags, 2),
        // property 'elapsedSeconds'
        QtMocHelpers::PropertyData<int>(36, QMetaType::Int, QMC::DefaultPropertyFlags, 3),
        // property 'remainingSeconds'
        QtMocHelpers::PropertyData<int>(37, QMetaType::Int, QMC::DefaultPropertyFlags, 3),
        // property 'formattedTime'
        QtMocHelpers::PropertyData<QString>(38, QMetaType::QString, QMC::DefaultPropertyFlags, 3),
        // property 'progress'
        QtMocHelpers::PropertyData<double>(39, QMetaType::Double, QMC::DefaultPropertyFlags, 3),
        // property 'pomodoroFocusMinutes'
        QtMocHelpers::PropertyData<int>(40, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 4),
        // property 'pomodoroBreakMinutes'
        QtMocHelpers::PropertyData<int>(41, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 4),
        // property 'flowtimeBreakDivisor'
        QtMocHelpers::PropertyData<int>(42, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 4),
        // property 'hasPendingBreakOffer'
        QtMocHelpers::PropertyData<bool>(43, QMetaType::Bool, QMC::DefaultPropertyFlags, 5),
        // property 'suggestedBreakMinutes'
        QtMocHelpers::PropertyData<int>(44, QMetaType::Int, QMC::DefaultPropertyFlags, 5),
        // property 'activeSubjectId'
        QtMocHelpers::PropertyData<qint64>(45, QMetaType::LongLong, QMC::DefaultPropertyFlags, 6),
        // property 'activeSubjectName'
        QtMocHelpers::PropertyData<QString>(46, QMetaType::QString, QMC::DefaultPropertyFlags, 6),
        // property 'activeSubjectColor'
        QtMocHelpers::PropertyData<QString>(47, QMetaType::QString, QMC::DefaultPropertyFlags, 6),
        // property 'backgroundType'
        QtMocHelpers::PropertyData<enum BackgroundType>(48, 0x80000000 | 49, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet, 7),
        // property 'backgroundColor'
        QtMocHelpers::PropertyData<QString>(50, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 7),
        // property 'backgroundMediaUrl'
        QtMocHelpers::PropertyData<QUrl>(51, 0x80000000 | 52, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet, 7),
        // property 'audioTrackUrl'
        QtMocHelpers::PropertyData<QUrl>(53, 0x80000000 | 52, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet, 7),
        // property 'audioTrackName'
        QtMocHelpers::PropertyData<QString>(54, QMetaType::QString, QMC::DefaultPropertyFlags, 7),
        // property 'audioVolume'
        QtMocHelpers::PropertyData<double>(55, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 7),
        // property 'audioMuted'
        QtMocHelpers::PropertyData<bool>(56, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 7),
        // property 'shouldPlayFocusAudio'
        QtMocHelpers::PropertyData<bool>(57, QMetaType::Bool, QMC::DefaultPropertyFlags, 8),
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'TimerMode'
        QtMocHelpers::EnumData<enum TimerMode>(31, 31, QMC::EnumFlags{}).add({
            {   58, TimerMode::Pomodoro },
            {   59, TimerMode::Flowtime },
        }),
        // enum 'TimerPhase'
        QtMocHelpers::EnumData<enum TimerPhase>(33, 33, QMC::EnumFlags{}).add({
            {   60, TimerPhase::Focus },
            {   61, TimerPhase::Break },
        }),
        // enum 'BackgroundType'
        QtMocHelpers::EnumData<enum BackgroundType>(49, 49, QMC::EnumFlags{}).add({
            {   62, BackgroundType::SolidColor },
            {   63, BackgroundType::Image },
            {   64, BackgroundType::Video },
        }),
    };
    return QtMocHelpers::metaObjectData<FocusTimerViewModel, qt_meta_tag_ZN7cadence19FocusTimerViewModelE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject cadence::FocusTimerViewModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7cadence19FocusTimerViewModelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7cadence19FocusTimerViewModelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7cadence19FocusTimerViewModelE_t>.metaTypes,
    nullptr
} };

void cadence::FocusTimerViewModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<FocusTimerViewModel *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->modeChanged(); break;
        case 1: _t->phaseChanged(); break;
        case 2: _t->runningChanged(); break;
        case 3: _t->timeChanged(); break;
        case 4: _t->configChanged(); break;
        case 5: _t->breakOfferChanged(); break;
        case 6: _t->activeSubjectChanged(); break;
        case 7: _t->mediaSettingsChanged(); break;
        case 8: _t->audioPlaybackStateChanged(); break;
        case 9: _t->focusSessionLogged((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 10: _t->breakFinished(); break;
        case 11: _t->onTick(); break;
        case 12: _t->start(); break;
        case 13: _t->pause(); break;
        case 14: _t->toggleStartPause(); break;
        case 15: _t->reset(); break;
        case 16: _t->completeAndLogSession(); break;
        case 17: _t->acceptFlowtimeBreak(); break;
        case 18: _t->dismissFlowtimeBreak(); break;
        case 19: _t->skipBreak(); break;
        case 20: _t->selectSubject((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 21: _t->clearBackgroundMedia(); break;
        case 22: _t->clearAudioTrack(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (FocusTimerViewModel::*)()>(_a, &FocusTimerViewModel::modeChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (FocusTimerViewModel::*)()>(_a, &FocusTimerViewModel::phaseChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (FocusTimerViewModel::*)()>(_a, &FocusTimerViewModel::runningChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (FocusTimerViewModel::*)()>(_a, &FocusTimerViewModel::timeChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (FocusTimerViewModel::*)()>(_a, &FocusTimerViewModel::configChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (FocusTimerViewModel::*)()>(_a, &FocusTimerViewModel::breakOfferChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (FocusTimerViewModel::*)()>(_a, &FocusTimerViewModel::activeSubjectChanged, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (FocusTimerViewModel::*)()>(_a, &FocusTimerViewModel::mediaSettingsChanged, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (FocusTimerViewModel::*)()>(_a, &FocusTimerViewModel::audioPlaybackStateChanged, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (FocusTimerViewModel::*)(const QString & , int )>(_a, &FocusTimerViewModel::focusSessionLogged, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (FocusTimerViewModel::*)()>(_a, &FocusTimerViewModel::breakFinished, 10))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<enum TimerMode*>(_v) = _t->mode(); break;
        case 1: *reinterpret_cast<enum TimerPhase*>(_v) = _t->phase(); break;
        case 2: *reinterpret_cast<bool*>(_v) = _t->isRunning(); break;
        case 3: *reinterpret_cast<bool*>(_v) = _t->isPaused(); break;
        case 4: *reinterpret_cast<int*>(_v) = _t->elapsedSeconds(); break;
        case 5: *reinterpret_cast<int*>(_v) = _t->remainingSeconds(); break;
        case 6: *reinterpret_cast<QString*>(_v) = _t->formattedTime(); break;
        case 7: *reinterpret_cast<double*>(_v) = _t->progress(); break;
        case 8: *reinterpret_cast<int*>(_v) = _t->pomodoroFocusMinutes(); break;
        case 9: *reinterpret_cast<int*>(_v) = _t->pomodoroBreakMinutes(); break;
        case 10: *reinterpret_cast<int*>(_v) = _t->flowtimeBreakDivisor(); break;
        case 11: *reinterpret_cast<bool*>(_v) = _t->hasPendingBreakOffer(); break;
        case 12: *reinterpret_cast<int*>(_v) = _t->suggestedBreakMinutes(); break;
        case 13: *reinterpret_cast<qint64*>(_v) = _t->activeSubjectId(); break;
        case 14: *reinterpret_cast<QString*>(_v) = _t->activeSubjectName(); break;
        case 15: *reinterpret_cast<QString*>(_v) = _t->activeSubjectColor(); break;
        case 16: *reinterpret_cast<enum BackgroundType*>(_v) = _t->backgroundType(); break;
        case 17: *reinterpret_cast<QString*>(_v) = _t->backgroundColor(); break;
        case 18: *reinterpret_cast<QUrl*>(_v) = _t->backgroundMediaUrl(); break;
        case 19: *reinterpret_cast<QUrl*>(_v) = _t->audioTrackUrl(); break;
        case 20: *reinterpret_cast<QString*>(_v) = _t->audioTrackName(); break;
        case 21: *reinterpret_cast<double*>(_v) = _t->audioVolume(); break;
        case 22: *reinterpret_cast<bool*>(_v) = _t->audioMuted(); break;
        case 23: *reinterpret_cast<bool*>(_v) = _t->shouldPlayFocusAudio(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setMode(*reinterpret_cast<enum TimerMode*>(_v)); break;
        case 8: _t->setPomodoroFocusMinutes(*reinterpret_cast<int*>(_v)); break;
        case 9: _t->setPomodoroBreakMinutes(*reinterpret_cast<int*>(_v)); break;
        case 10: _t->setFlowtimeBreakDivisor(*reinterpret_cast<int*>(_v)); break;
        case 16: _t->setBackgroundType(*reinterpret_cast<enum BackgroundType*>(_v)); break;
        case 17: _t->setBackgroundColor(*reinterpret_cast<QString*>(_v)); break;
        case 18: _t->setBackgroundMediaUrl(*reinterpret_cast<QUrl*>(_v)); break;
        case 19: _t->setAudioTrackUrl(*reinterpret_cast<QUrl*>(_v)); break;
        case 21: _t->setAudioVolume(*reinterpret_cast<double*>(_v)); break;
        case 22: _t->setAudioMuted(*reinterpret_cast<bool*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *cadence::FocusTimerViewModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *cadence::FocusTimerViewModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7cadence19FocusTimerViewModelE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int cadence::FocusTimerViewModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 23)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 23;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 24;
    }
    return _id;
}

// SIGNAL 0
void cadence::FocusTimerViewModel::modeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void cadence::FocusTimerViewModel::phaseChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void cadence::FocusTimerViewModel::runningChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void cadence::FocusTimerViewModel::timeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void cadence::FocusTimerViewModel::configChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void cadence::FocusTimerViewModel::breakOfferChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void cadence::FocusTimerViewModel::activeSubjectChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void cadence::FocusTimerViewModel::mediaSettingsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void cadence::FocusTimerViewModel::audioPlaybackStateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void cadence::FocusTimerViewModel::focusSessionLogged(const QString & _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 9, nullptr, _t1, _t2);
}

// SIGNAL 10
void cadence::FocusTimerViewModel::breakFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}
QT_WARNING_POP
