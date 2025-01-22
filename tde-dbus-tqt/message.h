/* message.h: TQt wrapper for DBusMessage
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
#ifndef DBUS_TQT_MESSAGE_H
#define DBUS_TQT_MESSAGE_H

#include <tqvariant.h>
#include <tqstring.h>
#include <tqstringlist.h>

#include "dbus/dbus.h"

namespace DBusQt {

  class Message
  {
  public:
    class iterator {
    public:
      iterator();
      iterator( const iterator& );
      iterator( DBusMessage* msg );
      ~iterator();

      iterator& operator=( const iterator& );
      const TQVariant& operator*() const;
      TQVariant& operator*();
      iterator& operator++();
      iterator operator++(int);
      bool operator==( const iterator& it );
      bool operator!=( const iterator& it );

      TQVariant var() const;
    protected:
      TQVariant marshallBaseType( DBusMessageIter* i );
      void fillVar();
      struct IteratorData;
      IteratorData *d;
    };

    Message( int messageType );
    Message( DBusMessage * );//hide this one from the public implementation
    Message( const TQString& service, const TQString& path,
             const TQString& interface, const TQString& method );
    Message( const Message& replayingTo );
    Message( const TQString& path, const TQString& interface,
             const TQString& name );
    Message( const Message& replayingTo, const TQString& errorName,
             const TQString& errorMessage );

    Message operator=( const Message& other );

    virtual ~Message();

    int type() const;

    void setPath( const TQString& );
    TQString path() const;

    void setInterface( const TQString& );
    TQString interface() const;

    void setMember( const TQString& );
    TQString member() const;

    void setErrorName( const TQString& );
    TQString errorName() const;

    void setDestination( const TQString& );
    TQString destination() const;

    bool    setSender( const TQString& sender );
    TQString    sender() const;

    TQString signature() const;

    iterator begin() const;
    iterator end() const;

    TQVariant at( int i );


  public:
    Message& operator<<( bool );
    Message& operator<<( TQ_INT8 );
    Message& operator<<( TQ_INT32 );
    Message& operator<<( TQ_UINT32 );
    Message& operator<<( TQ_INT64 );
    Message& operator<<( TQ_UINT64 );
    Message& operator<<( double );
    Message& operator<<( const TQString& );
    Message& operator<<( const TQVariant& );
    //Message& operator<<();
    //Message& operator<<();
    //Message& operator<<();
    //Message& operator<<();
    //Message& operator<<();
    //Message& operator<<();
    //Message& operator<<();

  protected:
    friend class Connection;
    DBusMessage* message() const;

  private:
    struct Private;
    Private *d;
  };

}

#endif
