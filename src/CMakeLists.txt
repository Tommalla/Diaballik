set (HEADERS
	gameConstants.h
	GraphicsScene.h
	GraphicsTile.h
	SettingsHandler.h
)

set (SOURCES
	main.cpp
	GraphicsScene.cpp
	GraphicsTile.cpp
	SettingsHandler.cpp
)

set (MOC_HEADERS
	GameHandler.h
	StateHandler.h
)

set (MOC_SOURCES
	GameHandler.cpp
	StateHandler.cpp
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
	${MOC_HEADERS}
	${MOC_SOURCES}
	${HEADERS}
	${SOURCES}
	${GUI_SOURCES} 
	${GUI_HEADERS}
	${FORMS}
)

include_directories (${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries (Diaballik ${QT_LIBRARIES})
target_link_libraries(Diaballik DiaballikEngine)

