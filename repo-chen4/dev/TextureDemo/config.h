/** CSci-4611 In-Class Example
*/


// The file config.h.in is processed by cmake to produce config.h.  This
// replaces strings of the form "at"CMAKE_VARIABLE_NAME"at" with the value
// of the corresponding cmake variable, allowing us to pass directory paths
// and other information configured with cmake into our C++ code.


#define DATA_DIR_BUILD "/Users/jinhaochen/Desktop/GitDesktop/SPRING2018/CSCI4611/repo-chen4566/dev/TextureDemo/data"
#define DATA_DIR_INSTALL "/Users/jinhaochen/Desktop/GitDesktop/SPRING2018/CSCI4611/repo-chen4566/dev/TextureDemo/build/install/share/example/data"

