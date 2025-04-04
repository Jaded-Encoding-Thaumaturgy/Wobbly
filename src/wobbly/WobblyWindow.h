/*

Copyright (c) 2015, John Smith

Permission to use, copy, modify, and/or distribute this software for
any purpose with or without fee is hereby granted, provided that the
above copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR
BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES
OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

*/


#ifndef WOBBLYWINDOW_H
#define WOBBLYWINDOW_H


#include <QCheckBox>
#include <QCloseEvent>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMainWindow>
#include <QMessageBox>
#include <QSettings>
#include <QSignalMapper>
#include <QSlider>
#include <QSpinBox>
#include <QStringListModel>

#include <VapourSynth4.h>
#include <VSScript4.h>

#include "DockWidget.h"
#include "FrameLabel.h"
#include "ImportWindow.h"
#include "ListWidget.h"
#include "OverlayLabel.h"
#include "PresetTextEdit.h"
#include "ScrollArea.h"
#include "SectionsProxyModel.h"
#include "SpinBox.h"
#include "TableView.h"
#include "TableWidget.h"
#include "WobblyProject.h"


#define MAX_THUMBNAILS 21


class WobblyWindow : public QMainWindow {
    Q_OBJECT

public:
    WobblyWindow();

private:
    // Menus.

    QMenu *tools_menu;

    QMenu *recent_menu;

    QAction *undo_action;
    QAction *redo_action;


    // Widgets.

    DockWidget *details_dock;
    QTabBar *tab_bar;
    FrameLabel *frame_label;
    ScrollArea *frame_scroll;
    QLabel *thumb_labels[MAX_THUMBNAILS];
    OverlayLabel *overlay_label;
    QSlider *frame_slider;

    QLabel *frame_num_label;
    QLabel *time_label;
    QLabel *matches_label;
    QLabel *section_label;
    QLabel *custom_list_label;
    QLabel *freeze_label;
    QLabel *decimate_metric_label;
    QLabel *mic_label;
    QLabel *mmetric_label;
    QLabel *vmetric_label;
    QLabel *pict_type_label;
    QLabel *combed_label;
    QLabel *bookmark_label;

    QLabel *selected_preset_label;
    QLabel *selected_custom_list_label;
    QLabel *zoom_label;

    DockWidget *crop_dock;
    QSpinBox *crop_spin[4];
    QGroupBox *crop_box;
    QCheckBox *crop_early_check;
    QSpinBox *resize_spin[2];
    QGroupBox *resize_box;
    QComboBox *resize_filter_combo;
    QGroupBox *depth_box;
    QComboBox *depth_bits_combo;
    QComboBox *depth_dither_combo;


    DockWidget *preset_dock;
    QComboBox *preset_combo;
    PresetTextEdit *preset_edit;

    DockWidget *pattern_dock;
    QLineEdit *match_pattern_edit;
    QLineEdit *decimation_pattern_edit;

    DockWidget *sections_dock;
    TableView *sections_view;
    SectionsProxyModel *sections_proxy_model;
    QGroupBox *short_sections_box;
    QSpinBox *short_sections_spin;
    ListWidget *section_presets_list;
    QListView *preset_list;

    DockWidget *cl_dock;
    TableView *cl_view;
    QComboBox *cl_presets_box;
    QButtonGroup *cl_position_group;
    TableView *cl_ranges_view;

    QButtonGroup *frame_rates_buttons;
    DockWidget *frame_rates_dock;
    TableWidget *frame_rates_table;

    DockWidget *frozen_frames_dock;
    TableView *frozen_frames_view;

    DockWidget *pg_dock;
    QSpinBox *pg_length_spin;
    QButtonGroup *pg_methods_buttons;
    QButtonGroup *pg_n_match_buttons;
    QButtonGroup *pg_decimate_buttons;
    QButtonGroup *pg_use_patterns_buttons;
    TableWidget *pg_failures_table;

    DockWidget *mic_search_dock;
    QSpinBox *mic_search_minimum_spin;

    DockWidget *dmetric_search_dock;
    QSpinBox *dmetric_search_minimum_spin;

    DockWidget *c_match_sequences_dock;
    QSpinBox *c_match_minimum_spin;
    TableWidget *c_match_sequences_table;

    DockWidget *fades_dock;
    QSpinBox *fades_gaps_spin;
    TableWidget *fades_table;

    DockWidget *combed_dock;
    TableView *combed_view;

    DockWidget *orphan_dock;
    TableView *orphan_view;

    DockWidget *bookmarks_dock;
    TableView *bookmarks_view;

    DockWidget *settings_dock;
    QSpinBox *settings_font_spin;
    QSpinBox *overlay_size_spin;
    QComboBox *application_style_combo;
    QCheckBox *settings_compact_projects_check;
    QCheckBox *settings_use_relative_paths_check;
    QComboBox *settings_colormatrix_combo;
    QSpinBox *settings_cache_spin;
    SpinBox *settings_undo_steps_spin;
    QCheckBox *settings_print_details_check;
    QCheckBox *settings_bookmark_description_check;
    QComboBox *settings_decimation_function_combo;
    SpinBox *settings_num_thumbnails_spin;
    QDoubleSpinBox *settings_thumbnail_size_dspin;
    TableWidget *settings_shortcuts_table;

    ImportWindow *import_window = nullptr;


    // Widget-related

    QImage splash_image;
    QPixmap splash_thumb;

    QString window_title;

    QSignalMapper *recent_menu_signal_mapper;


    // Other stuff.

    WobblyProject *project = nullptr;
    QString project_path;
    QString video_path;

    int current_frame = 0;
    int pending_frame = 0;
    int pending_requests = 0;
    VSNode *pending_requests_node = nullptr; // Don't free, it's just a copy.

    QString match_pattern;
    QString decimation_pattern;

    bool preview = false;

    struct Shortcut {
        QString keys;
        QString default_keys;
        QString description;
        void (WobblyWindow::* func)();
    };

    std::vector<Shortcut> shortcuts;

    int range_start = -1;
    int range_end = -1;

    int selected_preset = -1;
    int selected_custom_list = -1;

    QSettings settings;

    QString current_pict_type;
    int original_frame_width;
    int original_frame_height;


    // VapourSynth stuff.

    const VSAPI *vsapi = nullptr;
    const VSSCRIPTAPI *vssapi = nullptr;
    VSScript *vsscript = nullptr;
    VSCore *vscore = nullptr;
    VSNode *vsnode[2] = {};


    // Functions

    void createMenu();
    void createShortcuts();
    void resetShortcuts();
    void createFrameDetailsViewer();
    void createCropAssistant();
    void createPresetEditor();
    void createPatternEditor();
    void createSectionsEditor();
    void createCustomListsEditor();
    void createFrameRatesViewer();
    void createFrozenFramesViewer();
    void createPatternGuessingWindow();
    void createMicSearchWindow();
    void createDMetricSearchWindow();
    void createCMatchSequencesWindow();
    void createFadesWindow();
    void createCombedFramesWindow();
    void createOrphanFieldsWindow();
    void createBookmarksWindow();
    void createSettingsWindow();
    void createPluginWindow();
    void drawColorBars();
    void createUI();

    void addRecentFile(const QString &path);

    void readSettings();
    void writeSettings();

    void initialiseVapourSynth();
    void cleanUpVapourSynth();

    void updateGeometry();
    void updateWindowTitle();
    void initialiseCropAssistant();
    void initialisePresetEditor();
    void initialiseSectionsEditor();
    void initialiseCustomListsEditor();
    void updateFrameRatesViewer();
    void initialiseFrameRatesViewer();
    void initialiseFrozenFramesViewer();
    void updatePatternGuessingWindow();
    void initialisePatternGuessingWindow();
    void initialiseMicSearchWindow();
    void initialiseDMetricSearchWindow();
    void updateCMatchSequencesWindow();
    void initialiseCMatchSequencesWindow();
    void updateFadesWindow();
    void initialiseCombedFramesWindow();
    void initialiseOrphanFieldsWindow();
    void initialiseBookmarksWindow();
    void initialiseUIFromProject();

    void evaluateScript(bool final_script);
    void evaluateMainDisplayScript();
    void evaluateFinalScript();
    void requestFrames(int n);
    void updateFrameDetails();

    void errorPopup(const char *msg);

    void closeEvent(QCloseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);

    const char *getArgsForSourceFilter(const QString &source_filter);

    void realOpenProject(const QString &path);
    void realOpenVideo(const QString &path);
    void realSaveProject(const QString &path);
    void realSaveScript(const QString &path);
    void realSaveTimecodes(const QString &path);
    void realSaveSections(const QString &path);

    QMessageBox::StandardButton askToSaveIfModified();

    void jumpRelative(int offset);

    void zoom(bool in);

    void startRange();
    void finishRange();
    void cancelRange();

    int getSelectedPreset() const;
    void setSelectedPreset(int index);
    void selectPreviousPreset();
    void selectNextPreset();
    void assignSelectedPresetToCurrentSection();

    int getSelectedCustomList() const;
    void setSelectedCustomList(int index);
    void selectPreviousCustomList();
    void selectNextCustomList();
    void addRangeToSelectedCustomList();

    void copyCurrentFrameNumberToClipboard();
    void copyCurrentFrameImageToClipboard();

    QSize getThumbnailSize(QSize image_size);
    QPixmap getThumbnail(const QImage &image);

public slots:
    void jump1Forward();
    void jump1Backward();
    void jump5Forward();
    void jump5Backward();
    void jump50Forward();
    void jump50Backward();
    void jumpALotForward();
    void jumpALotBackward();

    void jumpToStart();
    void jumpToEnd();

    void jumpToNextSectionStart();
    void jumpToPreviousSectionStart();

    void jumpToPreviousMic();
    void jumpToNextMic();

    void jumpToPreviousDMetric();
    void jumpToNextDMetric();

    void jumpToPreviousBookmark();
    void jumpToNextBookmark();

    void jumpToFrame();

    void jumpToPreviousCombedFrame();
    void jumpToNextCombedFrame();

    void jumpToPreviousPatternFailureSection();
    void jumpToNextPatternFailureSection();

    void freezeForward();
    void freezeBackward();
    void freezeRange();

    void deleteFreezeFrame();
    void toggleFreezeFrames();

    void cycleMatchCNB();

    void toggleDecimation();

    void toggleCombed();

    void toggleBookmark();

    void addSection();
    void deleteSection();

    void openFile(const QString &path);
    void openProject();
    void openVideo();
    void saveProject();
    void saveProjectAs();
    void saveScript();
    void saveScriptAs();
    void saveTimecodes();
    void saveTimecodesAs();
    void saveSections();
    void saveSectionsAs();
    void saveScreenshot();
    void importFromProject();
    void quit();
    void showHideFrameDetails();
    void showHideCropping();
    void showHidePresets();
    void showHidePatternEditor();
    void showHideSections();
    void showHideCustomLists();
    void showHideFrameRates();
    void showHideFrozenFrames();
    void showHidePatternGuessing();
    void showHideMicSearchWindow();
    void showHideCMatchSequencesWindow();
    void showHideFadesWindow();
    void showHideCombedFramesWindow();
    void showHideOrphanFieldsWindow();
    void showHideBookmarksWindow();

    void showHideFrameDetailsOnVideo();

    void presetChanged(const QString &text);
    void presetEdited();

    void resetMatch();
    void resetSection();

    void rotateAndSetPatterns();
    void setMatchPattern();
    void setDecimationPattern();
    void setMatchAndDecimationPatterns();

    void updateSectionOrphanFields(int frame);
    void updateSectionOrphanFields(const Section *section);

    void guessCurrentSectionPatternsFromMics();
    void guessProjectPatternsFromMics();
    void guessCurrentSectionPatternsFromDMetrics();
    void guessCurrentSectionPatternsFromMicsAndDMetrics();
    void guessProjectPatternsFromDMetrics();
    void guessProjectPatternsFromMicsAndDMetrics();
    void guessCurrentSectionPatternsFromMatches();
    void guessProjectPatternsFromMatches();

    void togglePreview();

    void zoomIn();
    void zoomOut();

    void undo();
    void redo();
    void commit(std::string message);
    void updateUndoActions();
    void updateAfterUndo();

    void vsLogPopup(int msgType, const QString &msg);
    void frameDone(void *framev, int n, bool preview_node, const QString &errorMsg);
};

#endif // WOBBLYWINDOW_H
