/*
 * Cantata
 *
 * Copyright (c) 2011-2017 Craig Drummond <craig.p.drummond@gmail.com>
 *
 * ----
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "stdactions.h"
#include "widgets/icons.h"
#include "support/action.h"
#include "support/actioncollection.h"
#include "support/monoicon.h"
#include "models/playlistsmodel.h"
#ifdef ENABLE_DEVICES_SUPPORT
#include "models/devicesmodel.h"
#endif
#include "support/icon.h"
#include "widgets/icons.h"
#include "support/utils.h"
#include "widgets/mirrormenu.h"
#include "support/globalstatic.h"
#include <QCoreApplication>

GLOBAL_STATIC(StdActions, instance)

static void setToolTip(Action *act, const QString &tt)
{
    act->setToolTip(tt);
    act->setProperty(Action::constTtForSettings, true);
}

static QMenu * priorityMenu()
{
    Action *prioHighestAction = new Action(QObject::tr("Highest Priority (255)"), 0);
    Action *prioHighAction = new Action(QObject::tr("High Priority (200)"), 0);
    Action *prioMediumAction = new Action(QObject::tr("Medium Priority (125)"), 0);
    Action *prioLowAction = new Action(QObject::tr("Low Priority (50)"), 0);
    Action *prioDefaultAction = new Action(QObject::tr("Default Priority (0)"), 0);
    Action *prioCustomAction = new Action(QObject::tr("Custom Priority..."), 0);

    prioHighestAction->setData(255);
    prioHighAction->setData(200);
    prioMediumAction->setData(125);
    prioLowAction->setData(50);
    prioDefaultAction->setData(0);
    prioCustomAction->setData(-1);

    QMenu *prioMenu=new QMenu();
    prioMenu->addAction(prioHighestAction);
    prioMenu->addAction(prioHighAction);
    prioMenu->addAction(prioMediumAction);
    prioMenu->addAction(prioLowAction);
    prioMenu->addAction(prioDefaultAction);
    prioMenu->addAction(prioCustomAction);
    return prioMenu;
}

StdActions::StdActions()
{
    QColor col=Utils::monoIconColor();
    prevTrackAction = ActionCollection::get()->createAction("prevtrack", QObject::tr("Previous Track"), Icons::self()->toolbarPrevIcon);
    nextTrackAction = ActionCollection::get()->createAction("nexttrack", QObject::tr("Next Track"), Icons::self()->toolbarNextIcon);
    playPauseTrackAction = ActionCollection::get()->createAction("playpausetrack", QObject::tr("Play/Pause"), Icons::self()->toolbarPlayIcon);
    stopPlaybackAction = ActionCollection::get()->createAction("stopplayback", QObject::tr("Stop"), Icons::self()->toolbarStopIcon);
    stopAfterCurrentTrackAction = ActionCollection::get()->createAction("stopaftercurrenttrack", QObject::tr("Stop After Current Track"), Icons::self()->toolbarStopIcon);
    stopAfterTrackAction = ActionCollection::get()->createAction("stopaftertrack", QObject::tr("Stop After Track"), Icons::self()->toolbarStopIcon);
    increaseVolumeAction = ActionCollection::get()->createAction("increasevolume", QObject::tr("Increase Volume"));
    decreaseVolumeAction = ActionCollection::get()->createAction("decreasevolume", QObject::tr("Decrease Volume"));
    savePlayQueueAction = ActionCollection::get()->createAction("saveplayqueue", QObject::tr("Save As"), Icons::self()->savePlayQueueIcon);
    appendToPlayQueueAction = ActionCollection::get()->createAction("appendtoplayqueue", QObject::tr("Append"), Icons::self()->appendToPlayQueueIcon);
    setToolTip(appendToPlayQueueAction, QObject::tr("Append To Play Queue"));
    appendToPlayQueueAndPlayAction = ActionCollection::get()->createAction("appendtoplayqueueandplay", QObject::tr("Append And Play"));
    addToPlayQueueAndPlayAction = ActionCollection::get()->createAction("addtoplayqueueandplay", QObject::tr("Add And Play"));
    setToolTip(appendToPlayQueueAndPlayAction, QObject::tr("Append To Play Queue And Play"));
    insertAfterCurrentAction = ActionCollection::get()->createAction("insertintoplayqueue", QObject::tr("Insert After Current"));
    addRandomAlbumToPlayQueueAction = ActionCollection::get()->createAction("addrandomalbumtoplayqueue", QObject::tr("Append Random Album"));
    replacePlayQueueAction = ActionCollection::get()->createAction("replaceplayqueue", QObject::tr("Play Now (And Replace Play Queue)"), Icons::self()->replacePlayQueueIcon);
    savePlayQueueAction->setShortcut(Qt::ControlModifier+Qt::Key_S);
    appendToPlayQueueAction->setShortcut(Qt::ControlModifier+Qt::Key_P);
    replacePlayQueueAction->setShortcut(Qt::ControlModifier+Qt::Key_R);

    addWithPriorityAction = new Action(QObject::tr("Add With Priority"), 0);
    setPriorityAction = new Action(QObject::tr("Set Priority"), 0);
    setPriorityAction->setMenu(priorityMenu());
    addWithPriorityAction->setMenu(priorityMenu());

    addToStoredPlaylistAction = new Action(Icons::self()->playlistListIcon, QObject::tr("Add To Playlist"), 0);
    #ifdef TAGLIB_FOUND
    organiseFilesAction = ActionCollection::get()->createAction("orgfiles", QObject::tr("Organize Files"), MonoIcon::icon(FontAwesome::folderopeno, col));
    editTagsAction = ActionCollection::get()->createAction("edittags", QObject::tr("Edit Track Information"), 0);
    #endif
    #ifdef ENABLE_REPLAYGAIN_SUPPORT
    replaygainAction = ActionCollection::get()->createAction("replaygain", QObject::tr("ReplayGain"), MonoIcon::icon(FontAwesome::barchart, col));
    #endif
    #ifdef ENABLE_DEVICES_SUPPORT
    copyToDeviceAction = new Action(MonoIcon::icon(FontAwesome::mobile, col), QObject::tr("Copy Songs To Device"), 0);
    copyToDeviceAction->setMenu(DevicesModel::self()->menu());
    deleteSongsAction = new Action(MonoIcon::icon(FontAwesome::trash, MonoIcon::constRed), QObject::tr("Delete Songs"), 0);
    #endif
    setCoverAction = new Action(QObject::tr("Set Image"), 0);
    removeAction = new Action(Icons::self()->removeIcon, QObject::tr("Remove"), 0);
    searchAction = ActionCollection::get()->createAction("search", QObject::tr("Find"), Icons::self()->searchIcon);
    searchAction->setShortcut(Qt::ControlModifier+Qt::Key_F);

    addToStoredPlaylistAction->setMenu(PlaylistsModel::self()->menu());

    QMenu *addMenu=new QMenu();
    addToPlayQueueMenuAction = new Action(QObject::tr("Add To Play Queue"), 0);
    addMenu->addAction(appendToPlayQueueAction);
    addMenu->addAction(appendToPlayQueueAndPlayAction);
    addMenu->addAction(addToPlayQueueAndPlayAction);
    addMenu->addAction(addWithPriorityAction);
    addMenu->addAction(insertAfterCurrentAction);
    addMenu->addAction(addRandomAlbumToPlayQueueAction);
    addToPlayQueueMenuAction->setMenu(addMenu);
    addRandomAlbumToPlayQueueAction->setVisible(false);
}

void StdActions::enableAddToPlayQueue(bool en)
{
    appendToPlayQueueAction->setEnabled(en);
    appendToPlayQueueAndPlayAction->setEnabled(en);
    addToPlayQueueAndPlayAction->setEnabled(en);
    insertAfterCurrentAction->setEnabled(en);
    replacePlayQueueAction->setEnabled(en);
    addToStoredPlaylistAction->setEnabled(en);
    addToPlayQueueMenuAction->setEnabled(en);
}
