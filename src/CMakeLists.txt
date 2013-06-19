set (HEADERS
	gameConstants.h
	gameEnums.h
	GraphicsScene.h
	PlayerInfo.h
	SaveHandler.h
	SettingsHandler.h
)

set (SOURCES
	main.cpp
	GraphicsScene.cpp
	PlayerInfo.cpp
	SaveHandler.cpp
	SettingsHandler.cpp
)

set (MOC_HEADERS
	AIPlayer.h
	GameHandler.h
	GraphicsMovableTile.h
	GraphicsTile.h
	HumanPlayer.h
	Player.h
	StateHandler.h
)

set (MOC_SOURCES
	AIPlayer.cpp
	GameHandler.cpp
	GraphicsMovableTile.cpp
	GraphicsTile.cpp
	HumanPlayer.cpp
	Player.cpp
	StateHandler.cpp
)

set (GUI_HEADERS
	MainWindow.h
	NewGameDialog.h
)

set (GUI_SOURCES
	MainWindow.cpp
	NewGameDialog.cpp
)

set (FORMS
	MainWindow.ui
	NewGameDialog.ui
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

