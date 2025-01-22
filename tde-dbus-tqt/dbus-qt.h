/*
 * dbus-qt.h TQt integration
 *
 * Copyright (C)  2002  DBus Developers
 *
 * Licensed under the Academic Free License version 2.1
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307  USA
 *
 */
#ifndef DBUS_TQT_H
#define DBUS_TQT_H

#include <dbus/dbus.h>
/*
 * Two approaches - one presented below a DBusQtConnection
 * object which is a TQt wrapper around DBusConnection
class DBusQtConnection : public TQObject {
  TQ_OBJECT
  
public:
  DBusQtConnection( const char *address=0, TQObject *parent=0,
                    const char *name=0 );

  bool         open( const char *address );
  bool         isConnected() const;
  int          numMessages() const;

public slots:
  void disconnect();
  void flush();
  void sendMessage( DBusMessage *message );

signals:
  void message( DBusMessage* message );
  void error( const char* error );
private:
  DBusConnection  *mConnection;
  TQSocketNotifier *mReadNotifier;
  TQSocketNotifier *mWriteNotifier;
};
 *
 * Second approach is to have a static TQt dispatcher like:
class DBusQtNotifier : public TQObject {
  TQ_OBJECT
  
public:
  static DBusQtNotifier* dbus_qt_notifier();
  void addConnection(DBusConnection* connection);
signals:
  void message (DBusConnection* connection, DBusMessage* message);

private:
  DBusQtNotifier(TQObject *parent);
private slots:
  void processNotifiers( int socket );
private:
  //implemented in terms of TQSocketNotifiers
  TQAsciiDict<DBusConnection> mReadNotifiers;
  TQAsciiDict<DBusConnection> mWriteNotifiers;
};
 *
 * First one gives us a full wrapper for DBusConnection (the TQt way),
 * the other exposes DBusConnection, so would be easier to maintain
 * and keep up while DBus evolves.
 *
 */

#endif /* DBUS_TQT_H */
