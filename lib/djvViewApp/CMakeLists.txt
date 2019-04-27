set(header
    AboutDialog.h
	AnnotateSystem.h
	AnnotateTool.h
    Application.h
	AudioSystem.h
    ColorPickerTool.h
    DebugTool.h
    Enum.h
    FileBrowserDialog.h
    FileSystem.h
    FileSettings.h
    HelpSystem.h
    HistogramTool.h
    ITool.h
    IViewSystem.h
    ImageSystem.h
    ImageView.h
    ImageViewSystem.h
    InformationTool.h
	MDIWidget.h
    MagnifierTool.h
    MainWindow.h
    Media.h
    MediaWidget.h
	NUXSettings.h
    NUXSettingsWidget.h
	NUXSystem.h
	PlaybackSettings.h
    PlaybackSettingsWidget.h
    PlaybackSystem.h
	PlaylistWidget.h
    RecentFilesDialog.h
	SDIWidget.h
    SettingsSystem.h
    SettingsDialog.h
    SystemLogDialog.h
    TimelineSlider.h
    ToolSystem.h
    ViewApp.h
	WindowSettings.h
    WindowSystem.h)
set(source
    AboutDialog.cpp
	AnnotateSystem.cpp
	AnnotateTool.cpp
    Application.cpp
	AudioSystem.cpp
    ColorPickerTool.cpp
    DebugTool.cpp
    Enum.cpp
    FileBrowserDialog.cpp
    FileSettings.cpp
    FileSystem.cpp
    HelpSystem.cpp
    HistogramTool.cpp
    ITool.cpp
    IViewSystem.cpp
    ImageSystem.cpp
    ImageView.cpp
    ImageViewSystem.cpp
    InformationTool.cpp
	MDIWidget.cpp
    MagnifierTool.cpp
    MainWindow.cpp
    Media.cpp
    MediaWidget.cpp
	NUXSettings.cpp
    NUXSettingsWidget.cpp
	NUXSystem.cpp
	PlaybackSettings.cpp
    PlaybackSettingsWidget.cpp
    PlaybackSystem.cpp
	PlaylistWidget.cpp
    RecentFilesDialog.cpp
	SDIWidget.cpp
    SettingsSystem.cpp
    SettingsDialog.cpp
    SystemLogDialog.cpp
    TimelineSlider.cpp
    ToolSystem.cpp
	WindowSettings.cpp
    WindowSystem.cpp)

add_library(djvViewApp ${header} ${source})
target_link_libraries(djvViewApp djvUIComponents djvDesktopApp)
set_target_properties(
    djvViewApp
    PROPERTIES
    FOLDER lib
    CXX_STANDARD 11)