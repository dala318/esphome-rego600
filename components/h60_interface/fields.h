/**
 * Arduino H60 parser.
 *
 * This software is licensed under the MIT License.
 *
 * Copyright (c) 2015 Matthijs Kooijman <matthijs@stdin.nl>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Field parsing functions
 */

#ifndef H60_INCLUDE_FIELDS_H
#define H60_INCLUDE_FIELDS_H


namespace h60 {


/**
 * Small utility to get the length of an array at compiletime.
 */
template<typename T, unsigned int sz>
inline unsigned int lengthof(const T (&)[sz]) { return sz; }

// Hack until https://github.com/arduino/Arduino/pull/1936 is merged.
// This appends the given number of bytes from the given C string to the
// given Arduino string, without requiring a trailing NUL.
// Requires that there _is_ room for nul-termination
static void concat_hack(String& s, const char *append, size_t n) {
  // Add null termination. Inefficient, but it works...
  char buf[n + 1];
  memcpy(buf, append, n);
  buf[n] = 0;
  s.concat(buf);
}

/**
 * The ParseResult<T> class wraps the result of a parse function. The type
 * of the result is passed as a template parameter and can be void to
 * not return any result.
 *
 * A ParseResult can either:
 *  - Return an error. In this case, err is set to an error message, ctx
 *    is optionally set to where the error occurred. The result (if any)
 *    and the next pointer are meaningless.
 *  - Return succesfully. In this case, err and ctx are NULL, result
 *    contains the result (if any) and next points one past the last
 *    byte processed by the parser.
 *
 * The ParseResult class has some convenience functions:
 *  - succeed(result): sets the result to the given value and returns
 *    the ParseResult again.
 *  - fail(err): Set the err member to the error message passed,
 *    optionally sets the ctx and return the ParseResult again.
 *  - until(next): Set the next member and return the ParseResult again.
 *
 * Furthermore, ParseResults can be implicitely converted to other
 * types. In this case, the error message, context and and next pointer are
 * conserved, the return value is reset to the default value for the
 * target type.
 *
 * Note that ctx points into the string being parsed, so it does not
 * need to be freed, lives as long as the original string and is
 * probably way longer that needed.
 */

// Superclass for ParseResult so we can specialize for void without
// having to duplicate all content
template <typename P, typename T>
struct _ParseResult {
  T result;

  P& succeed(T& result) {
    this->result = result; return *static_cast<P*>(this);
  }
  P& succeed(T&& result) {
    this->result = result;
    return *static_cast<P*>(this);
  }
};

// partial specialization for void result
template <typename P>
struct _ParseResult<P, void> {
};

// Actual ParseResult class
template <typename T>
struct ParseResult : public _ParseResult<ParseResult<T>, T> {
  const char *next = NULL;
  const __FlashStringHelper *err = NULL;
  const char *ctx = NULL;

  ParseResult& fail(const __FlashStringHelper *err, const char* ctx = NULL) {
    this->err = err;
    this->ctx = ctx;
    return *this;
  }
  ParseResult& until(const char *next) {
    this->next = next;
    return *this;
  }
  ParseResult() = default;
  ParseResult(const ParseResult& other) = default;

  template <typename T2>
  ParseResult(const ParseResult<T2>& other): next(other.next), err(other.err), ctx(other.ctx) { }

  /**
   * Returns the error, including context in a fancy multi-line format.
   * The start and end passed are the first and one-past-the-end
   * characters in the total parsed string. These are needed to properly
   * limit the context output.
   */
  String fullError(const char* start, const char* end) const {
    String res;
    if (this->ctx && start && end) {
      // Find the entire line surrounding the context
      const char *line_end = this->ctx;
      while(line_end < end && line_end[0] != '\r' && line_end[0] != '\n') ++line_end;
      const char *line_start = this->ctx;
      while(line_start > start && line_start[-1] != '\r' && line_start[-1] != '\n') --line_start;

      // We can now predict the context string length, so let String allocate
      // memory in advance
      res.reserve((line_end - line_start) + 2 + (this->ctx - line_start) + 1 + 2);

      // Write the line
      concat_hack(res, line_start, line_end - line_start);
      res += "\r\n";

      // Write a marker to point out ctx
      while (line_start++ < this->ctx)
        res += ' ';
      res += '^';
      res += "\r\n";
    }
    res += this->err;
    return res;
  }
};

/**
 * An OBIS id is 6 bytes, usually noted as a-b:c.d.e.f. Here we put them
 * in an array for easy parsing.
 */
struct ObisId {
  uint8_t v[6];

  constexpr ObisId(uint8_t a, uint8_t b = 255, uint8_t c = 255, uint8_t d = 255, uint8_t e = 255, uint8_t f = 255)
    : v{a, b, c, d, e, f} { };
  constexpr ObisId() : v() {} // Zeroes

  bool operator==(const ObisId &other) const {
    return memcmp(&v, &other.v, sizeof(v)) == 0;
  }
};








/**
 * ParsedData is a template for the result of parsing a Dsmr P1 message.
 * You pass the fields you want to add to it as template arguments.
 *
 * This template will then generate a class that extends all the fields
 * passed (the fields really are classes themselves). Since each field
 * class has a single member variable, with the same name as the field
 * class, all of these fields will be available on the generated class.
 *
 * In other words, if I have:
 *
 * using MyData = ParsedData<
 *  identification,
 *  equipment_id
 * >;
 *
 * MyData data;
 *
 * then I can refer to the fields like data.identification and
 * data.equipment_id normally.
 *
 * Furthermore, this class offers some helper methods that can be used
 * to loop over all the fields inside it.
 */
template<typename... Ts>
struct ParsedData;

/**
 * Base case: No fields present.
 */
template<>
struct ParsedData<> {
  ParseResult<void> __attribute__((__always_inline__)) parse_line_inlined(const ObisId& /* id */, const char *str, const char * /* end */) {
    // Parsing succeeded, but found no matching handler (so return
    // set the next pointer to show nothing was parsed).
    return ParseResult<void>().until(str);
  }

  template<typename F>
  void __attribute__((__always_inline__)) applyEach_inlined(F&& /* f */) {
    // Nothing to do
  }

  bool all_present_inlined() {
    return true;
  }
};

// Do not use F() for multiply-used strings (including strings used from
// multiple template instantiations), that would result in multiple
// instances of the string in the binary
static constexpr char DUPLICATE_FIELD[] H60_PROGMEM = "Duplicate field";

/**
 * General case: At least one typename is passed.
 */
template<typename T, typename... Ts>
struct ParsedData<T, Ts...> : public T, ParsedData<Ts...> {
  /**
   * This method is used by the parser to parse a single line. The
   * OBIS id of the line is passed, and this method recursively finds a
   * field with a matching id. If any, it calls it's parse method, which
   * parses the value and stores it in the field.
   */
  ParseResult<void> parse_line(const ObisId& id, const char *str, const char *end) {
    return parse_line_inlined(id, str, end);
  }

  /**
   * always_inline version of parse_line. This is a separate method, to
   * allow recursively inlining all calls, but still have a non-inlined
   * top-level parse_line method.
   */
  ParseResult<void> __attribute__((__always_inline__)) parse_line_inlined(const ObisId& id, const char *str, const char *end) {
    if (id == T::id) {
      if (T::present())
        return ParseResult<void>().fail((const __FlashStringHelper*)DUPLICATE_FIELD, str);
      T::present() = true;
      return T::parse(str, end);
    }
    return ParsedData<Ts...>::parse_line_inlined(id, str, end);
  }

  template<typename F>
  void applyEach(F&& f) {
    applyEach_inlined(f);
  }

  template<typename F>
  void  __attribute__((__always_inline__)) applyEach_inlined(F&& f) {
    T::apply(f);
    return ParsedData<Ts...>::applyEach_inlined(f);
  }

  /**
   * Returns true when all defined fields are present.
   */
  bool all_present() {
    return all_present_inlined();
  }

  bool all_present_inlined() {
    return T::present() && ParsedData<Ts...>::all_present_inlined();
  }
};


struct StringParser {
  static ParseResult<String> parse_string(size_t min, size_t max, const char *str, const char *end) {
    ParseResult<String> res;
    if (str >= end || *str != '(')
      return res.fail(F("Missing ("), str);

    const char *str_start = str + 1; // Skip (
    const char *str_end = str_start;

    while(str_end < end && *str_end != ')')
      ++str_end;

    if (str_end == end)
      return res.fail(F("Missing )"), str_end);

    size_t len = str_end - str_start;
    if (len < min || len > max)
      return res.fail(F("Invalid string length"), str_start);

    concat_hack(res.result, str_start, len);

    return res.until(str_end + 1); // Skip )
  }
};

// Do not use F() for multiply-used strings (including strings used from
// multiple template instantiations), that would result in multiple
// instances of the string in the binary
static constexpr char INVALID_NUMBER[] H60_PROGMEM = "Invalid number";
static constexpr char INVALID_UNIT[] H60_PROGMEM = "Invalid unit";

struct NumParser {
  static ParseResult<uint32_t> parse(size_t max_decimals, const char* unit, const char *str, const char *end) {
    ParseResult<uint32_t> res;
    if (str >= end || *str != '(')
      return res.fail(F("Missing ("), str);

    const char *num_start = str + 1; // Skip (
    const char *num_end = num_start;

    uint32_t value = 0;

    // Parse integer part
    while(num_end < end && !strchr("*.)", *num_end)) {
      if (*num_end < '0' || *num_end > '9')
        return res.fail((const __FlashStringHelper*)INVALID_NUMBER, num_end);
      value *= 10;
      value += *num_end - '0';
      ++num_end;
    }

    // Parse decimal part, if any
    if (max_decimals && num_end < end && *num_end == '.') {
      ++num_end;

      while(num_end < end && !strchr("*)", *num_end) && max_decimals) {
        --max_decimals;
        if (*num_end < '0' || *num_end > '9')
          return res.fail((const __FlashStringHelper*)INVALID_NUMBER, num_end);
        value *= 10;
        value += *num_end - '0';
        ++num_end;
      }
    }

    // Fill in missing decimals with zeroes
    while(max_decimals--)
      value *= 10;

    // If a unit was passed, check that the unit in the messages
    // messages the unit passed.
    if (unit && *unit) {
      if (num_end >= end || *num_end != '*')
        return res.fail(F("Missing unit"), num_end);
      const char *unit_start = ++num_end; // skip *
      while(num_end < end && *num_end != ')' && *unit) {
        // Next character in units do not match?
        if (*num_end++ != *unit++)
          return res.fail((const __FlashStringHelper*)INVALID_UNIT, unit_start);
      }
      // At the end of the message unit, but not the passed unit?
      if (*unit)
        return res.fail((const __FlashStringHelper*)INVALID_UNIT, unit_start);
    }

    if (num_end >= end || *num_end != ')')
      return res.fail(F("Extra data"), num_end);

    return res.succeed(value).until(num_end + 1); // Skip )
  }
};

struct ObisIdParser {
  static ParseResult<ObisId> parse(const char *str, const char *end) {
    // Parse a Obis ID of the form 1-2:3.4.5.6
    // Stops parsing on the first unrecognized character. Any unparsed
    // parts are set to 255.
    ParseResult<ObisId> res;
    ObisId& id = res.result;
    res.next = str;
    uint8_t part = 0;
    while (res.next < end) {
      char c = *res.next;

      if (c >= '0' && c <= '9') {
        uint8_t digit = c - '0';
        if (id.v[part] > 25 || (id.v[part] == 25 && digit > 5))
          return res.fail(F("Obis ID has number over 255"), res.next);
        id.v[part] = id.v[part] * 10 + digit;
      } else if (part == 0 && c == '-') {
        part++;
      } else if (part == 1 && c == ':') {
        part++;
      } else if (part > 1 && part < 5 && c == '.') {
        part++;
      } else {
        break;
      }
      ++res.next;
    }

    if (res.next == str)
      return res.fail(F("OBIS id Empty"), str);

    for (++part; part < 6; ++part)
      id.v[part] = 255;

    return res;
  }
};

// struct CrcParser {
//   static const size_t CRC_LEN = 4;

//   // Parse a crc value. str must point to the first of the four hex
//   // bytes in the CRC.
//   static ParseResult<uint16_t> parse(const char *str, const char *end) {
//     ParseResult<uint16_t> res;
//     // This should never happen with the code in this library, but
//     // check anyway
//     if (str + CRC_LEN > end)
//       return res.fail(F("No checksum found"), str);

//     // A bit of a messy way to parse the checksum, but all
//     // integer-parse functions assume nul-termination
//     char buf[CRC_LEN + 1];
//     memcpy(buf, str, CRC_LEN);
//     buf[CRC_LEN] = '\0';
//     char *endp;
//     uint16_t check = strtoul(buf, &endp, 16);

//     // See if all four bytes formed a valid number
//     if (endp != buf + CRC_LEN)
//       return res.fail(F("Incomplete or malformed checksum"), str);

//     res.next = str + CRC_LEN;
//     return res.succeed(check);
//   }
// };

struct P1Parser {
  /**
    * Parse a complete P1 telegram. The string passed should start
    * with '/' and run up to and including the ! and the following
    * four byte checksum. It's ok if the string is longer, the .next
    * pointer in the result will indicate the next unprocessed byte.
    */
  template <typename... Ts>
  static ParseResult<void> parse(ParsedData<Ts...> *data, const char *str, size_t n, bool unknown_error = false) {
    ParseResult<void> res;
    if (!n || str[0] != '/')
      return res.fail(F("Data should start with /"), str);

    // Skip /
    const char *data_start = str + 1;

    // // Look for ! that terminates the data
    const char *data_end = data_start;
    // uint16_t crc = _crc16_update(0, *str); // Include the / in CRC
    // while (data_end < str + n && *data_end != '!') {
    //   crc = _crc16_update(crc, *data_end);
    //   ++data_end;
    // }

    if (data_end >= str + n)
      return res.fail(F("No checksum found"), data_end);

    // crc = _crc16_update(crc, *data_end); // Include the ! in CRC

    // ParseResult<uint16_t> check_res = CrcParser::parse(data_end + 1, str + n);
    // if (check_res.err)
    //   return check_res;

    // // Check CRC
    // if (check_res.result != crc)
    //   return res.fail(F("Checksum mismatch"), data_end + 1);

    res = parse_data(data, data_start, data_end, unknown_error);
    // res.next = check_res.next;
    return res;
  }

  /**
   * Parse the data part of a message. Str should point to the first
   * character after the leading /, end should point to the ! before the
   * checksum. Does not verify the checksum.
   */
  template <typename... Ts>
  static ParseResult<void> parse_data(ParsedData<Ts...> *data, const char *str, const char *end, bool unknown_error = false) {
    ParseResult<void> res;
    // Split into lines and parse those
    const char *line_end = str, *line_start = str;

    // Parse ID line
    while (line_end < end) {
      if (*line_end == '\r' || *line_end == '\n') {
        // The first identification line looks like:
        // XXX5<id string>
        // The DSMR spec is vague on details, but in 62056-21, the X's
        // are a three-letter (registerd) manufacturer ID, the id
        // string is up to 16 chars of arbitrary characters and the
        // '5' is a baud rate indication. 5 apparently means 9600,
        // which DSMR 3.x and below used. It seems that DSMR 2.x
        // passed '3' here (which is mandatory for "mode D"
        // communication according to 62956-21), so we also allow
        // that. Apparently swedish meters use '9' for 115200. This code
        // used to check the format of the line somewhat, but for
        // flexibility (and since we do not actually parse the contents
        // of the line anyway), just allow anything now.
        //
        // Offer it for processing using the all-ones Obis ID, which
        // is not otherwise valid.
        ParseResult<void> tmp = data->parse_line(ObisId(255, 255, 255, 255, 255, 255), line_start, line_end);
        if (tmp.err)
          return tmp;
        line_start = ++line_end;
        break;
      }
      ++line_end;
    }

    // Parse data lines
    while (line_end < end) {
      if (*line_end == '\r' || *line_end == '\n') {
        ParseResult<void> tmp = parse_line(data, line_start, line_end, unknown_error);
        if (tmp.err)
          return tmp;
        line_start = line_end + 1;
      }
      line_end++;
    }

    if (line_end != line_start)
      return res.fail(F("Last dataline not CRLF terminated"), line_end);

    return res;
  }

  template <typename Data>
  static ParseResult<void> parse_line(Data *data, const char *line, const char *end, bool unknown_error) {
    ParseResult<void> res;
    if (line == end)
      return res;

    ParseResult<ObisId> idres = ObisIdParser::parse(line, end);
    if (idres.err)
      return idres;

    ParseResult<void> datares = data->parse_line(idres.result, idres.next, end);
    if (datares.err)
      return datares;

    // If datares.next didn't move at all, there was no parser for
    // this field, that's ok. But if it did move, but not all the way
    // to the end, that's an error.
    if (datares.next != idres.next && datares.next != end)
      return res.fail(F("Trailing characters on data line"), datares.next);
    else if (datares.next == idres.next && unknown_error)
      return res.fail(F("Unknown field"), line);

    return res.until(end);
  }
};








/**
 * Superclass for data items in a P1 message.
 */
template <typename T>
struct ParsedField {
  template <typename F>
  void apply(F& f) {
    f.apply(*static_cast<T*>(this));
  }
  // By defaults, fields have no unit
  static const char *unit() { return ""; }
};

template <typename T, size_t minlen, size_t maxlen>
struct StringField : ParsedField<T> {
  ParseResult<void> parse(const char *str, const char *end) {
    ParseResult<String> res = StringParser::parse_string(minlen, maxlen, str, end);
    if (!res.err)
      static_cast<T*>(this)->val() = res.result;
    return res;
  }
};

// A timestamp is essentially a string using YYMMDDhhmmssX format (where
// X is W or S for wintertime or summertime). Parsing this into a proper
// (UNIX) timestamp is hard to do generically. Parsing it into a
// single integer needs > 4 bytes top fit and isn't very useful (you
// cannot really do any calculation with those values). So we just parse
// into a string for now.
template <typename T>
struct TimestampField : StringField<T, 13, 13> { };

// Value that is parsed as a three-decimal float, but stored as an
// integer (by multiplying by 1000). Supports val() (or implicit cast to
// float) to get the original value, and int_val() to get the more
// efficient integer value. The unit() and int_unit() methods on
// FixedField return the corresponding units for these values.
struct FixedValue {
  operator float() { return val();}
  float val() { return _value / 1000.0;}
  uint32_t int_val() { return _value; }

  uint32_t _value;
};

// Floating point numbers in the message never have more than 3 decimal
// digits. To prevent inefficient floating point operations, we store
// them as a fixed-point number: an integer that stores the value in
// thousands. For example, a value of 1.234 kWh is stored as 1234. This
// effectively means that the integer value is the value in Wh. To allow
// automatic printing of these values, both the original unit and the
// integer unit is passed as a template argument.
template <typename T, const char *_unit, const char *_int_unit>
struct FixedField : ParsedField<T> {
  ParseResult<void> parse(const char *str, const char *end) {
    ParseResult<uint32_t> res = NumParser::parse(3, _unit, str, end);
    if (!res.err)
      static_cast<T*>(this)->val()._value = res.result;
    return res;
  }

  static const char *unit() { return _unit; }
  static const char *int_unit() { return _int_unit; }
};

struct TimestampedFixedValue : public FixedValue {
  String timestamp;
};

// Some numerical values are prefixed with a timestamp. This is simply
// both of them concatenated, e.g. 0-1:24.2.1(150117180000W)(00473.789*m3)
template <typename T, const char *_unit, const char *_int_unit>
struct TimestampedFixedField : public FixedField<T, _unit, _int_unit> {
  ParseResult<void> parse(const char *str, const char *end) {
    // First, parse timestamp
    ParseResult<String> res = StringParser::parse_string(13, 13, str, end);
    if (res.err)
      return res;

    static_cast<T*>(this)->val().timestamp = res.result;

    // Which is immediately followed by the numerical value
    return FixedField<T, _unit, _int_unit>::parse(res.next, end);
  }
};

// A integer number is just represented as an integer.
template <typename T, const char *_unit>
struct IntField : ParsedField<T> {
  ParseResult<void> parse(const char *str, const char *end) {
    ParseResult<uint32_t> res = NumParser::parse(0, _unit, str, end);
    if (!res.err)
      static_cast<T*>(this)->val() = res.result;
    return res;
  }

  static const char *unit() { return _unit; }
};

// A RawField is not parsed, the entire value (including any
// parenthesis around it) is returned as a string.
template <typename T>
struct RawField : ParsedField<T> {
  ParseResult<void> parse(const char *str, const char *end) {
    // Just copy the string verbatim value without any parsing
    concat_hack(static_cast<T*>(this)->val(), str, end - str);
    return ParseResult<void>().until(end);
  }
};

namespace fields {

struct units {
  // These variables are inside a struct, since that allows us to make
  // them constexpr and define their values here, but define the storage
  // in a cpp file. Global const(expr) variables have implicitly
  // internal linkage, meaning each cpp file that includes us will have
  // its own copy of the variable. Since we take the address of these
  // variables (passing it as a template argument), this would cause a
  // compiler warning. By putting these in a struct, this is prevented.
  static constexpr char none[] = "";
  static constexpr char kWh[] = "kWh";
  static constexpr char Wh[] = "Wh";
  static constexpr char kW[] = "kW";
  static constexpr char W[] = "W";
  static constexpr char V[] = "V";
  static constexpr char mV[] = "mV";
  static constexpr char A[] = "A";
  static constexpr char mA[] = "mA";
  static constexpr char m3[] = "m3";
  static constexpr char dm3[] = "dm3";
  static constexpr char GJ[] = "GJ";
  static constexpr char MJ[] = "MJ";
};

const uint8_t GAS_MBUS_ID = 1;
const uint8_t WATER_MBUS_ID = 2;
const uint8_t THERMAL_MBUS_ID = 3;
const uint8_t SLAVE_MBUS_ID = 4;

template <typename FieldT>
struct NameConverter {
  public:
    operator const __FlashStringHelper*() const { return reinterpret_cast<const __FlashStringHelper*>(&FieldT::name_progmem); }
};

#define DEFINE_FIELD(fieldname, value_t, obis, field_t, field_args...) \
  struct fieldname : field_t<fieldname, ##field_args> { \
    value_t fieldname; \
    bool fieldname ## _present = false; \
    static constexpr ObisId id = obis; \
    static constexpr char name_progmem[] H60_PROGMEM = #fieldname; \
    /* name field is for compatibility with a __FlashStringHelper *name \
     * field that was previously used, but no longer worked on newer gcc \
     * versions. The get_name() method should be used instead. \
     * TODO (breaking change): Remove name field, rename get_name() to \
     * name() and add deprecated get_name() that calls name(). */ \
    [[gnu::deprecated]] static constexpr NameConverter<h60::fields::fieldname> name = {}; \
    static const __FlashStringHelper *get_name() { return reinterpret_cast<const __FlashStringHelper*>(&name_progmem); } \
    value_t& val() { return fieldname; } \
    bool& present() { return fieldname ## _present; } \
  }

/* Meter identification. This is not a normal field, but a
 * specially-formatted first line of the message */
DEFINE_FIELD(identification, String, ObisId(255, 255, 255, 255, 255, 255), RawField);

/* Version information for P1 output */
DEFINE_FIELD(p1_version, String, ObisId(1, 3, 0, 2, 8), StringField, 2, 2);

/* Date-time stamp of the P1 message */
DEFINE_FIELD(timestamp, String, ObisId(0, 0, 1, 0, 0), TimestampField);

/* Equipment identifier */
DEFINE_FIELD(equipment_id, String, ObisId(0, 0, 96, 1, 1), StringField, 0, 96);

/* Meter Reading electricity delivered to client (Tariff 1) in 0,001 kWh */
DEFINE_FIELD(energy_delivered_tariff1, FixedValue, ObisId(1, 0, 1, 8, 1), FixedField, units::kWh, units::Wh);
/* Meter Reading electricity delivered to client (Tariff 2) in 0,001 kWh */
DEFINE_FIELD(energy_delivered_tariff2, FixedValue, ObisId(1, 0, 1, 8, 2), FixedField, units::kWh, units::Wh);
/* Meter Reading electricity delivered by client (Tariff 1) in 0,001 kWh */
DEFINE_FIELD(energy_returned_tariff1, FixedValue, ObisId(1, 0, 2, 8, 1), FixedField, units::kWh, units::Wh);
/* Meter Reading electricity delivered by client (Tariff 2) in 0,001 kWh */
DEFINE_FIELD(energy_returned_tariff2, FixedValue, ObisId(1, 0, 2, 8, 2), FixedField, units::kWh, units::Wh);

/* Tariff indicator electricity. The tariff indicator can also be used
 * to switch tariff dependent loads e.g boilers. This is the
 * responsibility of the P1 user */
DEFINE_FIELD(electricity_tariff, String, ObisId(0, 0, 96, 14, 0), StringField, 4, 4);

/* Actual electricity power delivered (+P) in 1 Watt resolution */
DEFINE_FIELD(power_delivered, FixedValue, ObisId(1, 0, 1, 7, 0), FixedField, units::kW, units::W);
/* Actual electricity power received (-P) in 1 Watt resolution */
DEFINE_FIELD(power_returned, FixedValue, ObisId(1, 0, 2, 7, 0), FixedField, units::kW, units::W);

/* The actual threshold Electricity in kW. Removed in 4.0.7 / 4.2.2 / 5.0 */
DEFINE_FIELD(electricity_threshold, FixedValue, ObisId(0, 0, 17, 0, 0), FixedField, units::kW, units::W);

/* Switch position Electricity (in/out/enabled). Removed in 4.0.7 / 4.2.2 / 5.0 */
DEFINE_FIELD(electricity_switch_position, uint8_t, ObisId(0, 0, 96, 3, 10), IntField, units::none);

/* Number of power failures in any phase */
DEFINE_FIELD(electricity_failures, uint32_t, ObisId(0, 0, 96, 7, 21), IntField, units::none);
/* Number of long power failures in any phase */
DEFINE_FIELD(electricity_long_failures, uint32_t, ObisId(0, 0, 96, 7, 9), IntField, units::none);

/* Power Failure Event Log (long power failures) */
DEFINE_FIELD(electricity_failure_log, String, ObisId(1, 0, 99, 97, 0), RawField);

/* Number of voltage sags in phase L1 */
DEFINE_FIELD(electricity_sags_l1, uint32_t, ObisId(1, 0, 32, 32, 0), IntField, units::none);
/* Number of voltage sags in phase L2 (polyphase meters only) */
DEFINE_FIELD(electricity_sags_l2, uint32_t, ObisId(1, 0, 52, 32, 0), IntField, units::none);
/* Number of voltage sags in phase L3 (polyphase meters only) */
DEFINE_FIELD(electricity_sags_l3, uint32_t, ObisId(1, 0, 72, 32, 0), IntField, units::none);

/* Number of voltage swells in phase L1 */
DEFINE_FIELD(electricity_swells_l1, uint32_t, ObisId(1, 0, 32, 36, 0), IntField, units::none);
/* Number of voltage swells in phase L2 (polyphase meters only) */
DEFINE_FIELD(electricity_swells_l2, uint32_t, ObisId(1, 0, 52, 36, 0), IntField, units::none);
/* Number of voltage swells in phase L3 (polyphase meters only) */
DEFINE_FIELD(electricity_swells_l3, uint32_t, ObisId(1, 0, 72, 36, 0), IntField, units::none);

/* Text message codes: numeric 8 digits (Note: Missing from 5.0 spec)
 * */
DEFINE_FIELD(message_short, String, ObisId(0, 0, 96, 13, 1), StringField, 0, 16);
/* Text message max 2048 characters (Note: Spec says 1024 in comment and
 * 2048 in format spec, so we stick to 2048). */
DEFINE_FIELD(message_long, String, ObisId(0, 0, 96, 13, 0), StringField, 0, 2048);

/* Instantaneous voltage L1 in 0.1V resolution (Note: Spec says V
 * resolution in comment, but 0.1V resolution in format spec. Added in
 * 5.0) */
DEFINE_FIELD(voltage_l1, FixedValue, ObisId(1, 0, 32, 7, 0), FixedField, units::V, units::mV);
/* Instantaneous voltage L2 in 0.1V resolution (Note: Spec says V
 * resolution in comment, but 0.1V resolution in format spec. Added in
 * 5.0) */
DEFINE_FIELD(voltage_l2, FixedValue, ObisId(1, 0, 52, 7, 0), FixedField, units::V, units::mV);
/* Instantaneous voltage L3 in 0.1V resolution (Note: Spec says V
 * resolution in comment, but 0.1V resolution in format spec. Added in
 * 5.0) */
DEFINE_FIELD(voltage_l3, FixedValue, ObisId(1, 0, 72, 7, 0), FixedField, units::V, units::mV);

/* Instantaneous current L1 in A resolution */
DEFINE_FIELD(current_l1, uint16_t, ObisId(1, 0, 31, 7, 0), IntField, units::A);
/* Instantaneous current L2 in A resolution */
DEFINE_FIELD(current_l2, uint16_t, ObisId(1, 0, 51, 7, 0), IntField, units::A);
/* Instantaneous current L3 in A resolution */
DEFINE_FIELD(current_l3, uint16_t, ObisId(1, 0, 71, 7, 0), IntField, units::A);

/* Instantaneous active power L1 (+P) in W resolution */
DEFINE_FIELD(power_delivered_l1, FixedValue, ObisId(1, 0, 21, 7, 0), FixedField, units::kW, units::W);
/* Instantaneous active power L2 (+P) in W resolution */
DEFINE_FIELD(power_delivered_l2, FixedValue, ObisId(1, 0, 41, 7, 0), FixedField, units::kW, units::W);
/* Instantaneous active power L3 (+P) in W resolution */
DEFINE_FIELD(power_delivered_l3, FixedValue, ObisId(1, 0, 61, 7, 0), FixedField, units::kW, units::W);

/* Instantaneous active power L1 (-P) in W resolution */
DEFINE_FIELD(power_returned_l1, FixedValue, ObisId(1, 0, 22, 7, 0), FixedField, units::kW, units::W);
/* Instantaneous active power L2 (-P) in W resolution */
DEFINE_FIELD(power_returned_l2, FixedValue, ObisId(1, 0, 42, 7, 0), FixedField, units::kW, units::W);
/* Instantaneous active power L3 (-P) in W resolution */
DEFINE_FIELD(power_returned_l3, FixedValue, ObisId(1, 0, 62, 7, 0), FixedField, units::kW, units::W);


/* Device-Type */
DEFINE_FIELD(gas_device_type, uint16_t, ObisId(0, GAS_MBUS_ID, 24, 1, 0), IntField, units::none);

/* Equipment identifier (Gas) */
DEFINE_FIELD(gas_equipment_id, String, ObisId(0, GAS_MBUS_ID, 96, 1, 0), StringField, 0, 96);

/* Valve position Gas (on/off/released) (Note: Removed in 4.0.7 / 4.2.2 / 5.0). */
DEFINE_FIELD(gas_valve_position, uint8_t, ObisId(0, GAS_MBUS_ID, 24, 4, 0), IntField, units::none);

/* Last 5-minute value (temperature converted), gas delivered to client
 * in m3, including decimal values and capture time (Note: 4.x spec has
 * "hourly value") */
DEFINE_FIELD(gas_delivered, TimestampedFixedValue, ObisId(0, GAS_MBUS_ID, 24, 2, 1), TimestampedFixedField, units::m3, units::dm3);


/* Device-Type */
DEFINE_FIELD(thermal_device_type, uint16_t, ObisId(0, THERMAL_MBUS_ID, 24, 1, 0), IntField, units::none);

/* Equipment identifier (Thermal: heat or cold) */
DEFINE_FIELD(thermal_equipment_id, String, ObisId(0, THERMAL_MBUS_ID, 96, 1, 0), StringField, 0, 96);

/* Valve position (on/off/released) (Note: Removed in 4.0.7 / 4.2.2 / 5.0). */
DEFINE_FIELD(thermal_valve_position, uint8_t, ObisId(0, THERMAL_MBUS_ID, 24, 4, 0), IntField, units::none);

/* Last 5-minute Meter reading Heat or Cold in 0,01 GJ and capture time
 * (Note: 4.x spec has "hourly meter reading") */
DEFINE_FIELD(thermal_delivered, TimestampedFixedValue, ObisId(0, THERMAL_MBUS_ID, 24, 2, 1), TimestampedFixedField, units::GJ, units::MJ);


/* Device-Type */
DEFINE_FIELD(water_device_type, uint16_t, ObisId(0, WATER_MBUS_ID, 24, 1, 0), IntField, units::none);

/* Equipment identifier (Thermal: heat or cold) */
DEFINE_FIELD(water_equipment_id, String, ObisId(0, WATER_MBUS_ID, 96, 1, 0), StringField, 0, 96);

/* Valve position (on/off/released) (Note: Removed in 4.0.7 / 4.2.2 / 5.0). */
DEFINE_FIELD(water_valve_position, uint8_t, ObisId(0, WATER_MBUS_ID, 24, 4, 0), IntField, units::none);

/* Last 5-minute Meter reading in 0,001 m3 and capture time
 * (Note: 4.x spec has "hourly meter reading") */
DEFINE_FIELD(water_delivered, TimestampedFixedValue, ObisId(0, WATER_MBUS_ID, 24, 2, 1), TimestampedFixedField, units::m3, units::dm3);


/* Device-Type */
DEFINE_FIELD(slave_device_type, uint16_t, ObisId(0, SLAVE_MBUS_ID, 24, 1, 0), IntField, units::none);

/* Equipment identifier (Thermal: heat or cold) */
DEFINE_FIELD(slave_equipment_id, String, ObisId(0, SLAVE_MBUS_ID, 96, 1, 0), StringField, 0, 96);

/* Valve position (on/off/released) (Note: Removed in 4.0.7 / 4.2.2 / 5.0). */
DEFINE_FIELD(slave_valve_position, uint8_t, ObisId(0, SLAVE_MBUS_ID, 24, 4, 0), IntField, units::none);

/* Last 5-minute Meter reading Heat or Cold and capture time (e.g. slave
 * E meter) (Note: 4.x spec has "hourly meter reading") */
DEFINE_FIELD(slave_delivered, TimestampedFixedValue, ObisId(0, SLAVE_MBUS_ID, 24, 2, 1), TimestampedFixedField, units::m3, units::dm3);

} // namespace fields

} // namespace h60

#endif // H60_INCLUDE_FIELDS_H