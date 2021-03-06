/***************************************************************************
    qgsoptionsdialogbase.h - base vertical tabs option dialog

    ---------------------
    begin                : March 24, 2013
    copyright            : (C) 2013 by Larry Shaffer
    email                : larrys at dakcarto dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSOPTIONSDIALOGBASE_H
#define QGSOPTIONSDIALOGBASE_H

#include "qgsguiutils.h"
#include "qgssettings.h"
#include "qgis_gui.h"

#include <functional>

#include <QDialog>
#include <QPointer>
#include <QStyledItemDelegate>

class QDialogButtonBox;
class QListWidget;
class QModelIndex;
class QPalette;
class QPainter;
class QStackedWidget;
class QStyleOptionViewItem;
class QSplitter;

class QgsFilterLineEdit;
class QgsOptionsDialogHighlightWidget;


/**
 * \ingroup gui
 * \class QgsOptionsDialogBase
 * A base dialog for options and properties dialogs that offers vertical tabs.
 * It handles saving/restoring of geometry, splitter and current tab states,
 * switching vertical tabs between icon/text to icon-only modes (splitter collapsed to left),
 * and connecting QDialogButtonBox's accepted/rejected signals to dialog's accept/reject slots
 *
 * To use:
 * 1) Start with copy of qgsoptionsdialog_template.ui and build options/properties dialog.
 * 2) In source file for dialog, inherit this class instead of QDialog, then in constructor:
 *    ...
 *    setupUi( this ); // set up .ui file objects
 *    initOptionsBase( FALSE ); // set up this class to use .ui objects, optionally restoring base ui
 *    ...
 *    restoreOptionsBaseUi(); // restore the base ui with initOptionsBase or use this later on
 */

class GUI_EXPORT QgsOptionsDialogBase : public QDialog
{
    Q_OBJECT

  public:

    /**
     * Constructor
     * \param settingsKey QgsSettings subgroup key for saving/restore ui states, e.g. "ProjectProperties".
     * \param parent parent object (owner)
     * \param fl widget flags
     * \param settings custom QgsSettings pointer
     */
    QgsOptionsDialogBase( const QString &settingsKey, QWidget *parent SIP_TRANSFERTHIS = nullptr, Qt::WindowFlags fl = nullptr, QgsSettings *settings = nullptr );
    ~QgsOptionsDialogBase() override;

    /**
     * Set up the base ui connections for vertical tabs.
     * \param restoreUi Whether to restore the base ui at this time.
     * \param title the window title
     */
    void initOptionsBase( bool restoreUi = true, const QString &title = QString() );

    // set custom QgsSettings pointer if dialog used outside QGIS (in plugin)
    void setSettings( QgsSettings *settings );

    /**
     * Restore the base ui.
     * Sometimes useful to do at end of subclass's constructor.
     * \param title the window title (it does not need to be defined if previously given to initOptionsBase();
     */
    void restoreOptionsBaseUi( const QString &title = QString() );

    /**
     * Refocus the active tab from the last time the dialog was shown.
     *
     * \since QGIS 3.14
     */
    void restoreLastPage();

    /**
     * Resizes all tabs when the dialog is resized
     * \param index current tab index
     * \since QGIS 3.10
     */
    void resizeAlltabs( int index );

    /**
     * Determine if the options list is in icon only mode
     */
    bool iconOnly() {return mIconOnly;}

    /**
     * Sets the dialog \a page (by object name) to show.
     *
     * \since QGIS 3.14
     */
    void setCurrentPage( const QString &page );

    /**
     * Adds a new page to the dialog pages.
     *
     * The \a title, \a tooltip and \a icon arguments dictate the page list item title, tooltip and icon respectively.
     *
     * The page content is specified via the \a widget argument. Ownership of \a widget is transferred to the dialog.
     *
     * \see insertPage()
     * \since QGIS 3.14
     */
    void addPage( const QString &title, const QString &tooltip, const QIcon &icon, QWidget *widget SIP_TRANSFER );

    /**
     * Inserts a new page into the dialog pages.
     *
     * The \a title, \a tooltip and \a icon arguments dictate the page list item title, tooltip and icon respectively.
     *
     * The page content is specified via the \a widget argument. Ownership of \a widget is transferred to the dialog.
     *
     * The \a before argument specifies the object name of an existing page. The new page will be inserted directly
     * before the matching page.
     *
     * \see addPage()
     * \since QGIS 3.14
     */
    void insertPage( const QString &title, const QString &tooltip, const QIcon &icon, QWidget *widget SIP_TRANSFER, const QString &before );

  public slots:

    /**
     * searchText searches for a text in all the pages of the stacked widget and highlight the results
     * \param text the text to search
     * \since QGIS 3.0
     */
    void searchText( const QString &text );

  protected slots:
    //! Update tabs on the splitter move
    virtual void updateOptionsListVerticalTabs();
    //! Select relevant tab on current page change
    virtual void optionsStackedWidget_CurrentChanged( int index );
    //! Remove tab and unregister widgets on page remove
    virtual void optionsStackedWidget_WidgetRemoved( int index );

    void warnAboutMissingObjects();

  protected:
    void showEvent( QShowEvent *e ) override;
    void paintEvent( QPaintEvent *e ) override;

    virtual void updateWindowTitle();

    /**
     * register widgets in the dialog to search for text in it
     * it is automatically called if a line edit has "mSearchLineEdit" as object name.
     * \since QGIS 3.0
     */
    void registerTextSearchWidgets();

    QList< QPair< QgsOptionsDialogHighlightWidget *, int > > mRegisteredSearchWidgets;

    QString mOptsKey;
    bool mInit;
    QListWidget *mOptListWidget = nullptr;
    QStackedWidget *mOptStackedWidget = nullptr;
    QSplitter *mOptSplitter = nullptr;
    QDialogButtonBox *mOptButtonBox = nullptr;
    QgsFilterLineEdit *mSearchLineEdit = nullptr;
    QString mDialogTitle;
    bool mIconOnly;
    // pointer to app or custom, external QgsSettings
    // QPointer in case custom settings obj gets deleted while dialog is open
    QPointer<QgsSettings> mSettings;
    bool mDelSettings;
};

#endif // QGSOPTIONSDIALOGBASE_H
