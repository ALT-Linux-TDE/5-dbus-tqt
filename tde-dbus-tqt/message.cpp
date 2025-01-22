/* message.cpp: TQt wrapper for DBusMessage
 *
 * Copyright (C) 2003  Zack Rusin <zack@kde.org>
 *
 * Licensed under the Academic Free License version 2.0
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
#include "message.h"

#include <tqmap.h>

#include <cstdlib>

namespace DBusQt {

struct Message::iterator::IteratorData {
  DBusMessageIter *iter;
  TQVariant         var;
  bool             end;
  DBusMessage	  *mesg;
};

/**
 * Iterator.
 */
Message::iterator::iterator()
{
  d = new IteratorData;
  d->iter = 0; d->end = true;
}

/**
 * Constructs iterator for the message.
 * @param msg message whose fields we want to iterate
 */
Message::iterator::iterator( DBusMessage* msg )
{
  d = new IteratorData;
  d->mesg = msg;
  d->iter = static_cast<DBusMessageIter *>( malloc( sizeof(DBusMessageIter) ) );
  dbus_message_iter_init( d->mesg, d->iter );
  if ( !d->iter ) {
    tqDebug("No iterator??");
  }
  fillVar();
  d->end = false;
}

/**
 * Copy constructor for the iterator.
 * @param itr iterator
 */
Message::iterator::iterator( const iterator& itr )
{
  d = new IteratorData;
  d->iter = itr.d->iter;
  d->var  = itr.d->var;
  d->end  = itr.d->end;
}

/**
 * Destructor.
 */
Message::iterator::~iterator()
{
  free( d->iter );
  delete d; d=0;
}

/**
 * Creates an iterator equal to the @p itr iterator
 * @param itr other iterator
 * @return
 */
Message::iterator&
Message::iterator::operator=( const iterator& itr )
{
  IteratorData *tmp = new IteratorData;
  tmp->iter = itr.d->iter;
  tmp->var  = itr.d->var;
  tmp->end  = itr.d->end;
  delete d; d=tmp;
  return *this;
}

/**
 * Returns the constant TQVariant held by the iterator.
 * @return the constant reference to TQVariant held by this iterator
 */
const TQVariant&
Message::iterator::operator*() const
{
  return d->var;
}

/**
 * Returns the TQVariant held by the iterator.
 * @return reference to TQVariant held by this iterator
 */
TQVariant&
Message::iterator::operator*()
{
  return d->var;
}

/**
 * Moves to the next field and return a reference to itself after
 * incrementing.
 * @return reference to self after incrementing
 */
Message::iterator&
Message::iterator::operator++()
{
  if ( d->end )
    return *this;

  if (  dbus_message_iter_next( d->iter ) ) {
    fillVar();
  } else {
    d->end = true;
    d->var = TQVariant();
  }
  return *this;
}

/**
 * Moves to the next field and returns self before incrementing.
 * @return self before incrementing
 */
Message::iterator
Message::iterator::operator++(int)
{
  iterator itr( *this );
  operator++();
  return itr;
}

/**
 * Compares this iterator to @p it iterator.
 * @param it the iterator to which we're comparing this one to
 * @return true if they're equal, false otherwise
 */
bool
Message::iterator::operator==( const iterator& it )
{
  if ( d->end == it.d->end ) {
    if ( d->end == true ) {
      return true;
    } else {
      return d->var == it.d->var;
    }
  } else
    return false;
}

/**
 * Compares two iterators.
 * @param it The other iterator.
 * @return true if two iterators are not equal, false
 *         otherwise
 */
bool
Message::iterator::operator!=( const iterator& it )
{
  return !operator==( it );
}

TQVariant Message::iterator::marshallBaseType( DBusMessageIter* i )
{
  TQVariant ret;
  switch (dbus_message_iter_get_arg_type(i)) {
  case DBUS_TYPE_INT32:
    {
      dbus_int32_t v;
      dbus_message_iter_get_basic (i, &v);
      ret = TQVariant( v );
    }
    break;
  case DBUS_TYPE_UINT32:
    {
      dbus_uint32_t v;
      dbus_message_iter_get_basic (i, &v);
      ret = TQVariant( v );
    }
    break;
  case DBUS_TYPE_DOUBLE:
    {
      double v;
      dbus_message_iter_get_basic (i, &v);
      ret = TQVariant( v );
    }
    break;
  case DBUS_TYPE_STRING:
    {
      const char *v;
      dbus_message_iter_get_basic (i, &v);
      ret = TQVariant( v );
    }
    break;
  default:
    ret = TQVariant();
    break;
  }
  return ret;
}

/**
 * Fills TQVariant based on what current DBusMessageIter helds.
 */
void
Message::iterator::fillVar()
{
  switch ( dbus_message_iter_get_arg_type( d->iter ) ) {
  case DBUS_TYPE_INT32:
  case DBUS_TYPE_UINT32:
  case DBUS_TYPE_DOUBLE:
  case DBUS_TYPE_STRING:
    d->var = marshallBaseType( d->iter );
    break;
  case DBUS_TYPE_ARRAY: {
    switch ( dbus_message_iter_get_element_type( d->iter ) ) {
    case DBUS_TYPE_STRING: {
      TQStringList tempList;
      DBusMessageIter sub;
      dbus_message_iter_recurse (d->iter, &sub);
      while (dbus_message_iter_get_arg_type (&sub) != DBUS_TYPE_INVALID)
        {
          const char *v;
          dbus_message_iter_get_basic (&sub, &v);
          tempList.append( TQString( v ) );
          dbus_message_iter_next (&sub);
        }
      d->var = TQVariant( tempList );
      break;
    }
    default:
      tqDebug( "Array of type not implemented" );
      d->var = TQVariant();
      break;
    }
    break;
  }
#if 0
  /* DICT is gone for now, but expected to be reintroduced, or else
   * reintroduced as a flag on the introspection data that can
   * apply to array of struct of two fields
   */
  case DBUS_TYPE_DICT: {
    tqDebug( "Got a hash!" );
    TQMap<TQString, TQVariant> tempMap;
    DBusMessageIter dictIter;
    dbus_message_iter_init_dict_iterator( d->iter, &dictIter );
    do {
      char *key = dbus_message_iter_get_dict_key( &dictIter );
      tempMap[key] = marshallBaseType( &dictIter );
      dbus_free( key );
      dbus_message_iter_next( &dictIter );
    } while( dbus_message_iter_has_next( &dictIter ) );
    d->var = TQVariant( tempMap );
    break;
    tqDebug( "Hash/Dict type not implemented" );
    d->var = TQVariant();
    break;
  }
#endif
  default:
    tqDebug( "not implemented" );
    d->var = TQVariant();
    break;
  }
}

/**
 * Returns a TQVariant help by this iterator.
 * @return TQVariant held by this iterator
 */
TQVariant
Message::iterator::var() const
{
  return d->var;
}

struct Message::Private {
  DBusMessage *msg;
};

Message::Message( DBusMessage *m )
{
  d = new Private;
  d->msg = m;
}

/**
 *
 */
Message::Message( int messageType )
{
  d = new Private;
  d->msg = dbus_message_new( messageType );
}

/**
 * Constructs a new Message with the given service and name.
 * @param service service service that the message should be sent to
 * @param name name of the message
 */
Message::Message( const TQString& service, const TQString& path,
                  const TQString& interface, const TQString& method )
{
  d = new Private;
  d->msg = dbus_message_new_method_call( service.latin1(), path.latin1(),
                                         interface.latin1(), method.latin1() );
}

/**
 * Constructs a message that is a reply to some other
 * message.
 * @param name the name of the message
 * @param replayingTo original_message the message which the created
 * message is a reply to.
 */
Message::Message( const Message& replayingTo )
{
  d = new Private;
  d->msg = dbus_message_new_method_return( replayingTo.d->msg );
}

Message:: Message( const TQString& path, const TQString& interface,
                   const TQString& name )
{
  d = new Private;
  d->msg = dbus_message_new_signal( path.ascii(), interface.ascii(),
                                    name.ascii() );
}

Message::Message( const Message& replayingTo, const TQString& errorName,
                  const TQString& errorMessage )
{
  d = new Private;
  d->msg = dbus_message_new_error( replayingTo.d->msg, errorName.utf8(),
                                   errorMessage.utf8() );
}

Message Message::operator=( const Message& other )
{
  //FIXME: ref the other.d->msg instead of copying it?
  return *this;
}
/**
 * Destructs message.
 */
Message::~Message()
{
  if ( d->msg ) {
    dbus_message_unref( d->msg );
  }
  delete d; d=0;
}

int Message::type() const
{
  return dbus_message_get_type( d->msg );
}

void Message::setPath( const TQString& path )
{
  dbus_message_set_path( d->msg, path.ascii() );
}

TQString Message::path() const
{
  return dbus_message_get_path( d->msg );
}

void Message::setInterface( const TQString& iface )
{
  dbus_message_set_interface( d->msg, iface.ascii() );
}

TQString Message::interface() const
{
  return dbus_message_get_interface( d->msg );
}

void Message::setMember( const TQString& member )
{
  dbus_message_set_member( d->msg, member.ascii() );
}

TQString Message::member() const
{
  return dbus_message_get_member( d->msg );
}

void Message::setErrorName( const TQString& err )
{
  dbus_message_set_error_name( d->msg, err.ascii() );
}

TQString Message::errorName() const
{
  return dbus_message_get_error_name( d->msg );
}

void Message::setDestination( const TQString& dest )
{
  dbus_message_set_destination( d->msg, dest.ascii() );
}

TQString Message::destination() const
{
  return dbus_message_get_destination( d->msg );
}

/**
 * Sets the message sender.
 * @param sender the sender
 * @return false if unsuccessful
 */
bool
Message::setSender( const TQString& sender )
{
  return dbus_message_set_sender( d->msg, sender.latin1() );
}

/**
 * Returns sender of this message.
 * @return sender
 */
TQString
Message::sender() const
{
  return dbus_message_get_sender( d->msg );
}

TQString Message::signature() const
{
  return dbus_message_get_signature( d->msg );
}


/**
 * Returns the starting iterator for the fields of this
 * message.
 * @return starting iterator
 */
Message::iterator
Message::begin() const
{
  return iterator( d->msg );
}

/**
 * Returns the ending iterator for the fields of this
 * message.
 * @return ending iterator
 */
Message::iterator
Message::end() const
{
  return iterator();
}

/**
 * Returns the field at position @p i
 * @param i position of the wanted field
 * @return TQVariant at position @p i or an empty TQVariant
 */
TQVariant
Message::at( int i )
{
  iterator itr( d->msg );

  while ( i-- ) {
    if ( itr == end() )
      return TQVariant();//nothing there
    ++itr;
  }
  return *itr;
}

/**
 * The underlying DBusMessage of this class.
 * @return DBusMessage pointer.
 */
DBusMessage*
Message::message() const
{
  return d->msg;
}

Message& Message::operator<<( bool b )
{
  const dbus_bool_t right_size_bool = b;
  dbus_message_append_args( d->msg, DBUS_TYPE_BOOLEAN, &right_size_bool,
                            DBUS_TYPE_INVALID );
  return *this;
}

Message& Message::operator<<( TQ_INT8 byte )
{
  dbus_message_append_args( d->msg, DBUS_TYPE_BYTE, &byte,
                            DBUS_TYPE_INVALID );
  return *this;
}

Message& Message::operator<<( TQ_INT32 num )
{
  dbus_message_append_args( d->msg, DBUS_TYPE_INT32, &num,
                            DBUS_TYPE_INVALID );
  return *this;
}

Message& Message::operator<<( TQ_UINT32 num )
{
  dbus_message_append_args( d->msg, DBUS_TYPE_UINT32, &num,
                            DBUS_TYPE_INVALID );
  return *this;
}

Message& Message::operator<<( TQ_INT64 num )
{
  dbus_message_append_args( d->msg, DBUS_TYPE_INT64, &num,
                            DBUS_TYPE_INVALID );
  return *this;
}

Message& Message::operator<<( TQ_UINT64 num )
{
  dbus_message_append_args( d->msg, DBUS_TYPE_UINT64, &num,
                            DBUS_TYPE_INVALID );
  return *this;
}

Message& Message::operator<<( double num )
{
  dbus_message_append_args( d->msg, DBUS_TYPE_DOUBLE, &num,
                            DBUS_TYPE_INVALID );
  return *this;
}

Message& Message::operator<<( const TQString& str )
{
  const char *u = str.utf8();
  dbus_message_append_args( d->msg, DBUS_TYPE_STRING, &u,
                            DBUS_TYPE_INVALID );
  return *this;
}

Message& Message::operator<<( const TQVariant& custom )
{
  //FIXME: imeplement
  return *this;
}

}
