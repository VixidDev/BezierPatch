/****************************************************************************
** Meta object code from reading C++ file 'RenderController.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "RenderController.h"
#include <QtGui/qtextcursor.h>
#include <QtGui/qscreen.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RenderController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.6.2. It"
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

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSRenderControllerENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSRenderControllerENDCLASS = QtMocHelpers::stringData(
    "RenderController",
    "objectRotationChanged",
    "",
    "xTranslateChanged",
    "value",
    "yTranslateChanged",
    "zTranslateChanged",
    "showNetCheckChanged",
    "state",
    "showPlanesCheckChanged",
    "showVerticesBoxCheckChanged",
    "showBezierBoxChanged",
    "orthoBoxChanged",
    "BeginScaledDrag",
    "whichButton",
    "x",
    "y",
    "ContinueScaledDrag",
    "EndScaledDrag"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSRenderControllerENDCLASS_t {
    uint offsetsAndSizes[38];
    char stringdata0[17];
    char stringdata1[22];
    char stringdata2[1];
    char stringdata3[18];
    char stringdata4[6];
    char stringdata5[18];
    char stringdata6[18];
    char stringdata7[20];
    char stringdata8[6];
    char stringdata9[23];
    char stringdata10[28];
    char stringdata11[21];
    char stringdata12[16];
    char stringdata13[16];
    char stringdata14[12];
    char stringdata15[2];
    char stringdata16[2];
    char stringdata17[19];
    char stringdata18[14];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSRenderControllerENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSRenderControllerENDCLASS_t qt_meta_stringdata_CLASSRenderControllerENDCLASS = {
    {
        QT_MOC_LITERAL(0, 16),  // "RenderController"
        QT_MOC_LITERAL(17, 21),  // "objectRotationChanged"
        QT_MOC_LITERAL(39, 0),  // ""
        QT_MOC_LITERAL(40, 17),  // "xTranslateChanged"
        QT_MOC_LITERAL(58, 5),  // "value"
        QT_MOC_LITERAL(64, 17),  // "yTranslateChanged"
        QT_MOC_LITERAL(82, 17),  // "zTranslateChanged"
        QT_MOC_LITERAL(100, 19),  // "showNetCheckChanged"
        QT_MOC_LITERAL(120, 5),  // "state"
        QT_MOC_LITERAL(126, 22),  // "showPlanesCheckChanged"
        QT_MOC_LITERAL(149, 27),  // "showVerticesBoxCheckChanged"
        QT_MOC_LITERAL(177, 20),  // "showBezierBoxChanged"
        QT_MOC_LITERAL(198, 15),  // "orthoBoxChanged"
        QT_MOC_LITERAL(214, 15),  // "BeginScaledDrag"
        QT_MOC_LITERAL(230, 11),  // "whichButton"
        QT_MOC_LITERAL(242, 1),  // "x"
        QT_MOC_LITERAL(244, 1),  // "y"
        QT_MOC_LITERAL(246, 18),  // "ContinueScaledDrag"
        QT_MOC_LITERAL(265, 13)   // "EndScaledDrag"
    },
    "RenderController",
    "objectRotationChanged",
    "",
    "xTranslateChanged",
    "value",
    "yTranslateChanged",
    "zTranslateChanged",
    "showNetCheckChanged",
    "state",
    "showPlanesCheckChanged",
    "showVerticesBoxCheckChanged",
    "showBezierBoxChanged",
    "orthoBoxChanged",
    "BeginScaledDrag",
    "whichButton",
    "x",
    "y",
    "ContinueScaledDrag",
    "EndScaledDrag"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSRenderControllerENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   86,    2, 0x0a,    1 /* Public */,
       3,    1,   87,    2, 0x0a,    2 /* Public */,
       5,    1,   90,    2, 0x0a,    4 /* Public */,
       6,    1,   93,    2, 0x0a,    6 /* Public */,
       7,    1,   96,    2, 0x0a,    8 /* Public */,
       9,    1,   99,    2, 0x0a,   10 /* Public */,
      10,    1,  102,    2, 0x0a,   12 /* Public */,
      11,    1,  105,    2, 0x0a,   14 /* Public */,
      12,    1,  108,    2, 0x0a,   16 /* Public */,
      13,    3,  111,    2, 0x0a,   18 /* Public */,
      17,    2,  118,    2, 0x0a,   22 /* Public */,
      18,    2,  123,    2, 0x0a,   25 /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int, QMetaType::Float, QMetaType::Float,   14,   15,   16,
    QMetaType::Void, QMetaType::Float, QMetaType::Float,   15,   16,
    QMetaType::Void, QMetaType::Float, QMetaType::Float,   15,   16,

       0        // eod
};

Q_CONSTINIT const QMetaObject RenderController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSRenderControllerENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSRenderControllerENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSRenderControllerENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<RenderController, std::true_type>,
        // method 'objectRotationChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'xTranslateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'yTranslateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'zTranslateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'showNetCheckChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'showPlanesCheckChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'showVerticesBoxCheckChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'showBezierBoxChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'orthoBoxChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'BeginScaledDrag'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<float, std::false_type>,
        QtPrivate::TypeAndForceComplete<float, std::false_type>,
        // method 'ContinueScaledDrag'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<float, std::false_type>,
        QtPrivate::TypeAndForceComplete<float, std::false_type>,
        // method 'EndScaledDrag'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<float, std::false_type>,
        QtPrivate::TypeAndForceComplete<float, std::false_type>
    >,
    nullptr
} };

void RenderController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RenderController *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->objectRotationChanged(); break;
        case 1: _t->xTranslateChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->yTranslateChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->zTranslateChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->showNetCheckChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->showPlanesCheckChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->showVerticesBoxCheckChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->showBezierBoxChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 8: _t->orthoBoxChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 9: _t->BeginScaledDrag((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<float>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<float>>(_a[3]))); break;
        case 10: _t->ContinueScaledDrag((*reinterpret_cast< std::add_pointer_t<float>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<float>>(_a[2]))); break;
        case 11: _t->EndScaledDrag((*reinterpret_cast< std::add_pointer_t<float>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<float>>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject *RenderController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RenderController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSRenderControllerENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int RenderController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP
