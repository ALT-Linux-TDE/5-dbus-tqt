/* connection.h: TQt wrapper for DBusConnection
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
#ifndef DBUS_TQT_CONNECTION_H
#define DBUS_TQT_CONNECTION_H

#include "message.h"

#include <tqobject.h>
#include <tqstring.h>

#include "dbus/dbus.h"

namespace DBusQt {
  namespace Internal {
    class Integrator;
  }

  class Connection : public TQObject
  {
    TQ_OBJECT
    
  public:
    Connection( TQObject *parent =0 );
    Connection( const TQString& host,
                TQObject *parent = 0 );
    Connection( DBusBusType type, TQObject* parent = 0 );

    bool isConnected() const;
    bool isAuthenticated() const;

    Message borrowMessage();
    Message popMessage();
    void stealBorrowMessage( const Message& );
    void dbus_connection_setup_with_qt_main (DBusConnection *connection);

  public slots:
    void open( const TQString& );
    void close();
    void flush();
    void send( const Message& );
    void sendWithReply( const Message& );
    Message sendWithReplyAndBlock( const Message& );

  protected slots:
    void dispatchRead();

  protected:
    void init( const TQString& host );
    virtual void *virtual_hook( int id, void *data );

  private:
    friend class Internal::Integrator;
    DBusConnection *connection() const;
    Connection( DBusConnection *connection, TQObject *parent );

  private:
    struct Private;
    Private *d;
  };

}


#endif
