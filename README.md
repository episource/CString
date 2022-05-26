# About
CString provides common string operations without need for (dynamic) heap allocations. It operates on a fixed size, pre-allocated (global variable or stack) buffer. The size of the buffer and the maximum number of contained strings (i.e. separate buffer areas) is configurable via template arguments.

# Example
```c++
// allocate buffer with capacity of 1024 characters divided in up to 10 strings
CStringBuffer<1024, 10> _buf; 
CString cstr = _buf.allocate();
cstr += "Hello";
cstr += " ";
cstr += "World";
cstr += "!";
```

Have a look at the unit tests for further examples.

# Important Notes
- A CString is only valid until the buffer used to allocate the string is in scope
- Using a CString, that is based on a buffer that went out of scope is undefined behavior. **Take care that the underlying buffer does not go out of scope!**
- The `char*` pointer returned by `raw()` is valid at least until the next interaction with the underlying buffer. This can be an interaction with any other CString allocated using this buffer. After such an operation, the pointer may be invalid, that is the CString's raw content may have been relocated within the buffer.
- All CString methods except `clone` modify the CString. `clone` allocates a new CString using the same buffer.
- CString operations that change the length of the contained string might trigger a relocation of the associated buffer area within the underlying buffer.
