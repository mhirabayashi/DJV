set(header
    AboutDialog.h
	Annotate.h
	AnnotateSettings.h
	AnnotateSystem.h
	AnnotateWidget.h
    Application.h
	ApplicationSettings.h
	AudioSystem.h
	BackgroundImageWidget.h
    ColorPickerSettings.h
    ColorPickerSystem.h
    ColorPickerWidget.h
	ColorSpaceWidget.h
	ColorSpaceWidgetPrivate.h
    DebugWidget.h
    EditSystem.h
    Enum.h
    FileSystem.h
    FileSettings.h
    FileSettingsWidget.h
    HelpSystem.h
    HistogramWidget.h
	HUDWidget.h
    IToolSystem.h
    IViewSystem.h
    ImageControlsWidget.h
	ImageSettings.h
    ImageSystem.h
    InfoWidget.h
	KeyboardSettings.h
	KeyboardSettingsWidget.h
	LayersWidget.h
	MDIWidget.h
    MagnifySettings.h
    MagnifySystem.h
    MagnifyWidget.h
    MainWindow.h
    Media.h
	MediaCanvas.h
	MediaWidget.h
	MediaWidgetPrivate.h
	MemoryCacheWidget.h
    MessagesWidget.h
	MouseSettings.h
	MouseSettingsWidget.h
	NUXSettings.h
    NUXSettingsWidget.h
	NUXSystem.h
	PlaybackSettings.h
    PlaybackSettingsWidget.h
    PlaybackSystem.h
    RecentFilesDialog.h
    SettingsSystem.h
    SettingsWidget.h
    SystemLogWidget.h
    TimelinePIPWidget.h
    TimelineSlider.h
    ToolSettings.h
    ToolSystem.h
	View.h
    ViewApp.h
    ViewControlsWidget.h
    ViewControlsWidgetPrivate.h
	ViewSettings.h
    ViewSystem.h
    ViewWidget.h
    ViewWidgetPrivate.h
	WindowSettings.h
	WindowSettingsWidget.h
    WindowSystem.h)
set(source
    AboutDialog.cpp
	Annotate.cpp
	AnnotateSettings.cpp
	AnnotateSystem.cpp
	AnnotateWidget.cpp
    Application.cpp
	ApplicationSettings.cpp
	AudioSystem.cpp
	BackgroundImageWidget.cpp
    ColorPickerSettings.cpp
    ColorPickerSystem.cpp
    ColorPickerWidget.cpp
	ColorSpaceConfigWidget.cpp
	ColorSpaceDisplayWidget.cpp
	ColorSpaceImageWidget.cpp
	ColorSpaceViewWidget.cpp
	ColorSpaceWidget.cpp
    DebugWidget.cpp
    EditSystem.cpp
    Enum.cpp
    FileSettings.cpp
    FileSettingsWidget.cpp
    FileSystem.cpp
    HelpSystem.cpp
    HistogramWidget.cpp
	HUDWidget.cpp
    IToolSystem.cpp
    IViewSystem.cpp
    ImageControlsWidget.cpp
	ImageSettings.cpp
    ImageSystem.cpp
    InfoWidget.cpp
	KeyboardSettings.cpp
	KeyboardSettingsWidget.cpp
	LayersWidget.cpp
	MDIWidget.cpp
    MagnifySettings.cpp
    MagnifySystem.cpp
    MagnifyWidget.cpp
    MainWindow.cpp
    Media.cpp
	MediaAudioWidget.cpp
	MediaCanvas.cpp
	MediaFrameWidget.cpp
    MediaPlaybackSpeedWidget.cpp
	MediaPointerWidget.cpp
	MediaWidget.cpp
	MemoryCacheWidget.cpp
    MessagesWidget.cpp
	MouseSettings.cpp
	MouseSettingsWidget.cpp
	NUXSettings.cpp
    NUXSettingsWidget.cpp
	NUXSystem.cpp
	PlaybackSettings.cpp
    PlaybackSettingsWidget.cpp
    PlaybackSystem.cpp
    RecentFilesDialog.cpp
    SettingsSystem.cpp
    SettingsWidget.cpp
    SystemLogWidget.cpp
    TimelinePIPWidget.cpp
    TimelineSlider.cpp
    ToolSettings.cpp
    ToolSystem.cpp
	View.cpp
    ViewControlsWidget.cpp
    ViewControlsWidgetPrivate.cpp
	ViewSettings.cpp
    ViewSystem.cpp
    ViewWidget.cpp
    ViewWidgetPrivate.cpp
	WindowSettings.cpp
	WindowSettingsWidget.cpp
    WindowSystem.cpp)

add_library(djvViewApp ${header} ${source})
target_link_libraries(djvViewApp djvUIComponents djvDesktopApp)
set_target_properties(
    djvViewApp
    PROPERTIES
    FOLDER lib
    CXX_STANDARD 11)

#install(
#    TARGETS djvViewApp
#    RUNTIME DESTINATION bin
#    LIBRARY DESTINATION lib
#    ARCHIVE DESTINATION lib)
#install(FILES ${header} DESTINATION include/djvViewApp)

