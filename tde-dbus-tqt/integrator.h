/* integrator.h: integrates D-BUS into TQt event loop
 *
 * Copyright (C) 2003  Zack Rusin <zack@kde.org>
 *
 * Licensed under the Academic Free License version 2.1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#ifndef DBUS_TQT_INTEGRATOR_H
#define DBUS_TQT_INTEGRATOR_H

#include <tqobject.h>

#include <tqintdict.h>
#include <tqptrdict.h>

#include "dbus/dbus.h"

class TQTimer;

namespace DBusQt
{
  class Connection;

  namespace Internal
  {
    struct Watch;

    class Timeout : public TQObject
    {
      TQ_OBJECT
      
    public:
      Timeout( TQObject *parent, DBusTimeout *t );
    public:
      void start();
    signals:
      void timeout( DBusTimeout* );
    protected slots:
      void slotTimeout();
    private:
      TQTimer *m_timer;
      DBusTimeout *m_timeout;
    };

    class Integrator : public TQObject
    {
      TQ_OBJECT
      
    public:
      Integrator( DBusConnection *connection, TQObject *parent );
      Integrator( DBusServer *server, TQObject *parent );

    signals:
      void readReady();
      void newConnection( Connection* );

    protected slots:
      void slotRead( int );
      void slotWrite( int );
      void slotTimeout( DBusTimeout *timeout );

    public:
      void addWatch( DBusWatch* );
      void removeWatch( DBusWatch* );

      void addTimeout( DBusTimeout* );
      void removeTimeout( DBusTimeout* );

      void handleConnection( DBusConnection* );
    private:
      TQIntDict<Watch> m_watches;
      TQPtrDict<Timeout> m_timeouts;
      DBusConnection *m_connection;
      DBusServer *m_server;
    };
  }
}

#endif
