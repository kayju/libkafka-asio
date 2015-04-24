//
// impl/message.h
// --------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef MESSAGE_H_AFAF4566_F008_41D9_962B_EE74729E4F4F
#define MESSAGE_H_AFAF4566_F008_41D9_962B_EE74729E4F4F

namespace libkafka_asio
{

inline Message::Message() :
  attributes_(0),
  nested_message_set_(new MessageSet())
{
}

inline Message::Message(const Message& orig, bool deep) :
  attributes_(orig.attributes_),
  nested_message_set_(orig.nested_message_set_)
{
  if (deep)
  {
    if (orig.key())
    {
      key_.reset(new Bytes::element_type(*orig.key()));
    }
    if (orig.value())
    {
      value_.reset(new Bytes::element_type(*orig.value()));
    }
    if (orig.nested_message_set_)
    {
      nested_message_set_.reset(new MessageSet(*orig.nested_message_set_));
    }
  }
  else
  {
    key_ = orig.key();
    value_ = orig.value();
    nested_message_set_ = orig.nested_message_set_;
  }
}

inline Message& Message::operator=(const Message& rhs)
{
  attributes_ = rhs.attributes_;
  key_ = rhs.key_;
  value_ = rhs.value_;
  nested_message_set_ = rhs.nested_message_set_;
  return *this;
}

inline Int8 Message::magic_byte() const
{
  return 1;
}

inline Int8 Message::attributes() const
{
  return attributes_;
}

inline const Bytes& Message::key() const
{
  return key_;
}

inline Bytes& Message::mutable_key()
{
  return key_;
}

inline const Bytes& Message::value() const
{
  return value_;
}

inline Bytes& Message::mutable_value()
{
  return value_;
}

inline const MessageSet& Message::nested_message_set() const
{
  return *nested_message_set_;
}

inline MessageSet& Message::mutable_nested_message_set()
{
  return *nested_message_set_;
}

inline void Message::SetCompression(Int8 attributes)
{
  Int8 compression_type = (attributes & 0x03);
  switch (compression_type)
  {
    case constants::kCompressionNone:
    case constants::kCompressionGZIP:
    case constants::kCompressionSnappy:
      attributes_ &= 0xfc;
      attributes_ |= compression_type;
      break;
  }
}

inline constants::Compression Message::compression() const
{
  return constants::Compression(attributes_ & 0x03);
}

inline MessageAndOffset::MessageAndOffset() :
  offset_(0)
{
}

inline MessageAndOffset::MessageAndOffset(const Message& message,
                                          Int64 offset) :
  Message(message),
  offset_(offset)
{
}

inline Int64 MessageAndOffset::offset() const
{
  return offset_;
}

inline void MessageAndOffset::set_offset(Int64 offset)
{
  offset_ = offset;
}

}  // namespace libkafka_asio

#endif  // MESSAGE_H_AFAF4566_F008_41D9_962B_EE74729E4F4F
