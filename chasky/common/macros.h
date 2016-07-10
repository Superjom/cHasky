
// avoid clang warnning
#define CH_PRINTF_ATTRIBUTE(string_index, first_to_check)                      \
  __attribute__((__format__(__printf__, string_index, first_to_check)))
#define CH_SCANF_ATTRIBUTE(string_index, first_to_check)                       \
  __attribute__((__format__(__scanf__, string_index, first_to_check)))

// disable class's copy and assign function
#define CH_DISALLOW_COPY_AND_ASSIGN(T)                                         \
  T(const T &) = delete;                                                       \
  void operator=(const T &) = delete;

#define CH_TEST_OR_RETURN_FALSE(COND)                                          \
  if (!(COND)) {                                                               \
    return false;                                                              \
  };

#define UN_IMPLEMENTED LOG(FATAL) << "UnImplemented";
