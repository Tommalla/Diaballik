set (HEADERS
	constants.h
	GraphicsScene.h
	SettingsHandler.h
)

set (SOURCES
	main.cpp
	GraphicsScene.cpp
	SettingsHandler.cpp
)

set (GUI_HEADERS
	MainWindow.h
)

set (GUI_SOURCES
	MainWindow.cpp
)

set (FORMS
	MainWindow.ui
)

qt4_wrap_cpp (GUI_SOURCES ${GUI_HEADERS})
qt4_wrap_cpp (MOC_SOURCES ${MOC_HEADERS})
qt4_wrap_ui (FORMS ${FORMS})

add_executable (Diaballik 
	${SOURCES}
	${GUI_SOURCES} 
	${GUI_HEADERS}
	${FORMS}
)

include_directories (${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries (Diaballik ${QT_LIBRARIES})
