QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS QT_NO_FOREACH

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Custom/customtablemodel.cpp \
    Custom/customtableviewmodel.cpp \
    Custom/roomstateitem.cpp \
    Custom/tablemodelpaging.cpp \
    DB/sqldatabase.cpp \
    DB/sqlhelper.cpp \
    DB/sqlinterface.cpp \
    Dao/basedao.cpp \
    Dao/speedydao.cpp \
    Delegate/lineeditdelegate.cpp \
    Delegate/spinboxdelegate.cpp \
    Delegate/sqlcomboboxdelegate.cpp \
    Delegate/usercalendardelegate.cpp \
    Delegate/usercomboboxdeletage.cpp \
    Delegate/userlineeditdelegate.cpp \
    Entitys/pagingclass.cpp \
    Service/ClientService/clientservice.cpp \
    Service/ReserveService/reserveservice.cpp \
    Service/ReserveService/roomstatemapservice.cpp \
    Service/SystemService/OtherManageService/billstatetypeservice.cpp \
    Service/SystemService/OtherManageService/chargeitemservice.cpp \
    Service/SystemService/OtherManageService/teminitialvalueservice.cpp \
    Service/SystemService/RoomManageService/roomattribueservice.cpp \
    Service/SystemService/RoomManageService/roomownattributeservice.cpp \
    Service/SystemService/RoomManageService/roomservice.cpp \
    Service/SystemService/RoomManageService/roomstateservice.cpp \
    Service/SystemService/RoomManageService/roomtypeservice.cpp \
    Service/SystemService/RoomManageService/towerrangeservie.cpp \
    Service/SystemService/RoomManageService/towerseatservice.cpp \
    Service/SystemService/StaffManageService/staffservice.cpp \
    Service/SystemService/StaffManageService/sysdepartmentservice.cpp \
    Service/SystemService/StaffManageService/sysoperatorservice.cpp \
    Service/SystemService/StaffManageService/sysstaffposservice.cpp \
    Service/comboboxitemservice.cpp \
    Widgets/ClientWidgets/client.cpp \
    Widgets/ClientWidgets/clientselectdialog.cpp \
    Widgets/Login/loginwidget.cpp \
    Widgets/MainWidget/centrewidget.cpp \
    Widgets/MainWidget/headerwidget.cpp \
    Widgets/MainWidget/modeltreewidget.cpp \
    Widgets/ReserveWidgets/reserveroom.cpp \
    Widgets/ReserveWidgets/reserveroomselectdialog.cpp \
    Widgets/ReserveWidgets/roomstateitemdatadialog.cpp \
    Widgets/ReserveWidgets/roomstatemap.cpp \
    Widgets/ReserveWidgets/roomstatemapdialog.cpp \
    Widgets/SystemWidget/OtherManageWidgets/sysbillstatetype.cpp \
    Widgets/SystemWidget/OtherManageWidgets/syschargeitem.cpp \
    Widgets/SystemWidget/OtherManageWidgets/systeminitialvalue.cpp \
    Widgets/SystemWidget/OtherManageWidgets/teminitialvaluedialog.cpp \
    Widgets/SystemWidget/RoomManageWidgets/room.cpp \
    Widgets/SystemWidget/RoomManageWidgets/roomfilterdialog.cpp \
    Widgets/SystemWidget/RoomManageWidgets/roomownattribute.cpp \
    Widgets/SystemWidget/RoomManageWidgets/sysroomattribute.cpp \
    Widgets/SystemWidget/RoomManageWidgets/sysroomstate.cpp \
    Widgets/SystemWidget/RoomManageWidgets/sysroomtype.cpp \
    Widgets/SystemWidget/RoomManageWidgets/systowerrange.cpp \
    Widgets/SystemWidget/RoomManageWidgets/systowerseat.cpp \
    Widgets/SystemWidget/StaffManageWidgets/staff.cpp \
    Widgets/SystemWidget/StaffManageWidgets/staffdialog.cpp \
    Widgets/SystemWidget/StaffManageWidgets/sysdepartment.cpp \
    Widgets/SystemWidget/StaffManageWidgets/sysoperator.cpp \
    Widgets/SystemWidget/StaffManageWidgets/sysstaffposition.cpp \
    Widgets/loadqss.cpp \
    main.cpp \
    widget.cpp

HEADERS += \
    Custom/customtablemodel.h \
    Custom/customtableviewmodel.h \
    Custom/roomstateitem.h \
    Custom/tablemodelpaging.h \
    DB/sqldatabase.h \
    DB/sqlhelper.h \
    DB/sqlinterface.h \
    Dao/basedao.h \
    Dao/speedydao.h \
    Delegate/lineeditdelegate.h \
    Delegate/spinboxdelegate.h \
    Delegate/sqlcomboboxdelegate.h \
    Delegate/usercalendardelegate.h \
    Delegate/usercomboboxdeletage.h \
    Delegate/userlineeditdelegate.h \
    Entitys/ClientEntitys.h \
    Entitys/ReserveInfo.h \
    Entitys/SysOtherEntitys.h \
    Entitys/SystemRoomManageEntitys.h \
    Entitys/SystemSimpleModelEntity.h \
    Entitys/comboboxentity.h \
    Entitys/pagingclass.h \
    Entitys/room.h \
    Service/ClientService/clientservice.h \
    Service/ReserveService/reserveservice.h \
    Service/ReserveService/roomstatemapservice.h \
    Service/SystemService/OtherManageService/billstatetypeservice.h \
    Service/SystemService/OtherManageService/chargeitemservice.h \
    Service/SystemService/OtherManageService/teminitialvalueservice.h \
    Service/SystemService/RoomManageService/roomattribueservice.h \
    Service/SystemService/RoomManageService/roomownattributeservice.h \
    Service/SystemService/RoomManageService/roomservice.h \
    Service/SystemService/RoomManageService/roomstateservice.h \
    Service/SystemService/RoomManageService/roomtypeservice.h \
    Service/SystemService/RoomManageService/towerrangeservie.h \
    Service/SystemService/RoomManageService/towerseatservice.h \
    Service/SystemService/StaffManageService/staffservice.h \
    Service/SystemService/StaffManageService/sysdepartmentservice.h \
    Service/SystemService/StaffManageService/sysoperatorservice.h \
    Service/SystemService/StaffManageService/sysstaffposservice.h \
    Service/comboboxitemservice.h \
    Widgets/ClientWidgets/client.h \
    Widgets/ClientWidgets/clientselectdialog.h \
    Widgets/Login/loginwidget.h \
    Widgets/MainWidget/centrewidget.h \
    Widgets/MainWidget/headerwidget.h \
    Widgets/MainWidget/modeltreewidget.h \
    Widgets/ReserveWidgets/reserveroom.h \
    Widgets/ReserveWidgets/reserveroomselectdialog.h \
    Widgets/ReserveWidgets/roomstateitemdatadialog.h \
    Widgets/ReserveWidgets/roomstatemap.h \
    Widgets/ReserveWidgets/roomstatemapdialog.h \
    Widgets/SystemWidget/OtherManageWidgets/sysbillstatetype.h \
    Widgets/SystemWidget/OtherManageWidgets/syschargeitem.h \
    Widgets/SystemWidget/OtherManageWidgets/systeminitialvalue.h \
    Widgets/SystemWidget/OtherManageWidgets/teminitialvaluedialog.h \
    Widgets/SystemWidget/RoomManageWidgets/room.h \
    Widgets/SystemWidget/RoomManageWidgets/roomfilterdialog.h \
    Widgets/SystemWidget/RoomManageWidgets/roomownattribute.h \
    Widgets/SystemWidget/RoomManageWidgets/sysroomattribute.h \
    Widgets/SystemWidget/RoomManageWidgets/sysroomstate.h \
    Widgets/SystemWidget/RoomManageWidgets/sysroomtype.h \
    Widgets/SystemWidget/RoomManageWidgets/systowerrange.h \
    Widgets/SystemWidget/RoomManageWidgets/systowerseat.h \
    Widgets/SystemWidget/StaffManageWidgets/staff.h \
    Widgets/SystemWidget/StaffManageWidgets/staffdialog.h \
    Widgets/SystemWidget/StaffManageWidgets/sysdepartment.h \
    Widgets/SystemWidget/StaffManageWidgets/sysoperator.h \
    Widgets/SystemWidget/StaffManageWidgets/sysstaffposition.h \
    Widgets/loadqss.h \
    widget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    Widgets/ClientWidgets/client.ui \
    Widgets/ClientWidgets/clientselectdialog.ui \
    Widgets/Login/loginwidget.ui \
    Widgets/MainWidget/centrewidget.ui \
    Widgets/MainWidget/headerwidget.ui \
    Widgets/MainWidget/modeltreewidget.ui \
    Widgets/ReserveWidgets/reserveroom.ui \
    Widgets/ReserveWidgets/reserveroomselectdialog.ui \
    Widgets/ReserveWidgets/roomstateitemdatadialog.ui \
    Widgets/ReserveWidgets/roomstatemap.ui \
    Widgets/ReserveWidgets/roomstatemapdialog.ui \
    Widgets/SystemWidget/OtherManageWidgets/sysbillstatetype.ui \
    Widgets/SystemWidget/OtherManageWidgets/syschargeitem.ui \
    Widgets/SystemWidget/OtherManageWidgets/systeminitialvalue.ui \
    Widgets/SystemWidget/OtherManageWidgets/teminitialvaluedialog.ui \
    Widgets/SystemWidget/RoomManageWidgets/room.ui \
    Widgets/SystemWidget/RoomManageWidgets/roomfilterdialog.ui \
    Widgets/SystemWidget/RoomManageWidgets/roomownattribute.ui \
    Widgets/SystemWidget/RoomManageWidgets/sysroomattribute.ui \
    Widgets/SystemWidget/RoomManageWidgets/sysroomstate.ui \
    Widgets/SystemWidget/RoomManageWidgets/sysroomtype.ui \
    Widgets/SystemWidget/RoomManageWidgets/systowerrange.ui \
    Widgets/SystemWidget/RoomManageWidgets/systowerseat.ui \
    Widgets/SystemWidget/StaffManageWidgets/staff.ui \
    Widgets/SystemWidget/StaffManageWidgets/staffdialog.ui \
    Widgets/SystemWidget/StaffManageWidgets/sysdepartment.ui \
    Widgets/SystemWidget/StaffManageWidgets/sysoperator.ui \
    Widgets/SystemWidget/StaffManageWidgets/sysstaffposition.ui \

RESOURCES += \
    Resources/Resources.qrc
