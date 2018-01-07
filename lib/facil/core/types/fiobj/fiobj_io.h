/*
Copyright: Boaz Segev, 2017-2018
License: MIT
*/
#if !defined(H_FIOBJ_IO_H) && (defined(__unix__) || defined(__APPLE__) ||      \
                               defined(__linux__) || defined(__CYGWIN__))

/**
 * A dynamic type for reading / writing to a local file,  a temporary file or an
 * in-memory string.
 *
 * Supports basic reak, write, seek, puts and gets operations.
 *
 * Writing is always performed at the end of the stream / memory buffer,
 * ignoring the current seek position.
 */
#define H_FIOBJ_IO_H

#include "fiobject.h"

#ifdef __cplusplus
extern "C" {
#endif

/* *****************************************************************************
Creating the IO object
***************************************************************************** */

/** Creates a new local in-memory IO object */
FIOBJ fiobj_io_newstr(void);

/**
 * Creates a IO object from an existing buffer. The buffer will be deallocated
 * using the provided `dealloc` function pointer. Use a NULL `dealloc` function
 * pointer if the buffer is static and shouldn't be freed.
 */
FIOBJ fiobj_io_newstr2(void *buffer, uintptr_t length, void (*dealloc)(void *));

/** Creates a new local tempfile IO object */
FIOBJ fiobj_io_newtmpfile(void);

/** Creates a new local file IO object */
FIOBJ fiobj_io_newfd(int fd);

/* *****************************************************************************
Reading API
***************************************************************************** */

/**
 * Reads up to `length` bytes and returns a temporary(!) buffer object (not NUL
 * terminated).
 *
 * If `length` is zero or negative, it will be computed from the end of the
 * input backwards (0 == EOF).
 *
 * The C string object will be invalidate the next time a function call to the
 * IO object is made.
 */
fio_cstr_s fiobj_io_read(FIOBJ io, intptr_t length);

/**
 * Reads until the `token` byte is encountered or until the end of the stream.
 *
 * Returns a temporary(!) C string including the end of line marker.
 *
 * Careful when using this call on large file streams, as the whole file
 * stream might be loaded into the memory.
 *
 * The C string object will be invalidate the next time a function call to the
 * IO object is made.
 */
fio_cstr_s fiobj_io_read2ch(FIOBJ io, uint8_t token);

/**
 * Reads a line (until the '\n' byte is encountered) or until the end of the
 * available data.
 *
 * Returns a temporary(!) buffer object (not NUL terminated) including the end
 * of line marker.
 *
 * Careful when using this call on large file streams, as the whole file stream
 * might be loaded into the memory.
 *
 * The C string object will be invalidate the next time a function call to the
 * IO object is made.
 */
#define fiobj_io_gets(io) fiobj_io_read2ch((io), '\n');

/**
 * Returns the current reading position. Returns -1 on error.
 */
intptr_t fiobj_io_pos(FIOBJ io);

/**
 * Moves the reading position to the requested position.
 */
void fiobj_io_seek(FIOBJ io, intptr_t position);

/**
 * Reads up to `length` bytes starting at `start_at` position and returns a
 * temporary(!) buffer object (not NUL terminated) string object. The reading
 * position is ignored and unchanged.
 *
 * The C string object will be invalidate the next time a function call to the
 * IO object is made.
 */
fio_cstr_s fiobj_io_pread(FIOBJ io, intptr_t start_at, uintptr_t length);

/* *****************************************************************************
Writing API
***************************************************************************** */

/**
 * Writes `length` bytes at the end of the IO stream, ignoring the reading
 * position.
 *
 * Behaves and returns the same value as the system call `write`.
 */
intptr_t fiobj_io_write(FIOBJ io, void *buffer, uintptr_t length);

/**
 * Writes `length` bytes at the end of the IO stream, ignoring the reading
 * position, adding an EOL marker ("\r\n") to the end of the stream.
 *
 * Behaves and returns the same value as the system call `write`.
 */
intptr_t fiobj_io_puts(FIOBJ io, void *buffer, uintptr_t length);

/**
 * Makes sure the IO object isn't attached to a static or external string.
 *
 * If the IO object is attached to a static or external string, the data will be
 * copied to a new memory block.
 */
void fiobj_io_assert_dynamic(FIOBJ io);

#if DEBUG
void fiobj_io_test(void);
#endif

#ifdef __cplusplus
}
#endif

#endif
