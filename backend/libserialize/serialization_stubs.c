#include <string.h>
#include <stdio.h>
#include <caml/callback.h>
#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>

/* --------------------
 * Initialize
 * -------------------- */

static const value* user_fn_bin2json_closure = NULL;
static const value* user_tipe_bin2json_closure = NULL;
static const value* handler_bin2json_closure = NULL;
static const value* db_bin2json_closure = NULL;
static const value* oplist_bin2json_closure = NULL;
static const value* pos_bin2json_closure = NULL;
static const value* user_fn_json2bin_closure = NULL;
static const value* user_tipe_json2bin_closure = NULL;
static const value* handler_json2bin_closure = NULL;
static const value* db_json2bin_closure = NULL;
static const value* oplist_json2bin_closure = NULL;
static const value* pos_json2bin_closure = NULL;
static const value* digest_value = NULL;


int check_string(value v) {
  if (Is_exception_result(v)) {
    printf("Value is exception!\n");
    return 0;
  }
  if (Tag_val(v) != String_tag) {
    printf("Value is expected to be a string but isn't!\n");
    return 0;
  }
  return 1;
}

char* copy_string_outside_runtime(value v) {
  if (check_string(v)) {
    return strndup(String_val(v), caml_string_length(v));
  }
  printf("Attempting to copy a non-string");
  exit(1);
}

extern char* dark_init_ocaml() {
  char* argv[2];
  argv[0] = "";
  argv[1] = NULL;
  printf("OCAML loaded!\n");
  value res = caml_startup_exn(argv);
  if (Is_exception_result(res)) {
    printf("OCAML exception !\n");
  }
  user_fn_bin2json_closure = caml_named_value("user_fn_bin2json");
  user_tipe_bin2json_closure = caml_named_value("user_tipe_bin2json");
  handler_bin2json_closure = caml_named_value("handler_bin2json");
  db_bin2json_closure = caml_named_value("db_bin2json");
  oplist_bin2json_closure = caml_named_value("oplist_bin2json");
  pos_bin2json_closure = caml_named_value("pos_bin2json");
  user_fn_json2bin_closure = caml_named_value("user_fn_json2bin");
  user_tipe_json2bin_closure = caml_named_value("user_tipe_json2bin");
  handler_json2bin_closure = caml_named_value("handler_json2bin");
  db_json2bin_closure = caml_named_value("db_json2bin");
  oplist_json2bin_closure = caml_named_value("oplist_json2bin");
  pos_json2bin_closure = caml_named_value("pos_json2bin");
  digest_value = caml_named_value("digest");
  check_string(*digest_value);
  return strdup("loaded");
}

/* --------------------
 * Convert from json to binary strings
 * //FSTODO check for memory leaks in these functions
 * -------------------- */
char* call_bin2json(const value* closure, void* bytes, int length) {
  if (Is_exception_result(*closure)) {
    printf("Closure is exception!\n");
  }
  value v = caml_alloc_initialized_string(length, bytes);
  check_string(v);
  value result = caml_callback(*closure, v);
  if (Is_exception_result(result)) {
    printf("result is exception!\n");
  }
  return copy_string_outside_runtime(result);
}
extern char* user_fn_bin2json(void* bytes, int length) {
  return call_bin2json(user_fn_bin2json_closure, bytes, length);
}
extern char* user_tipe_bin2json(void* bytes, int length) {
  return call_bin2json(user_tipe_bin2json_closure, bytes, length);
}
extern char* handler_bin2json(void* bytes, int length) {
  return call_bin2json(handler_bin2json_closure, bytes, length);
}
extern char* db_bin2json(void* bytes, int length) {
  return call_bin2json(db_bin2json_closure, bytes, length);
}
extern char* oplist_bin2json(void* bytes, int length) {
  return call_bin2json(oplist_bin2json_closure, bytes, length);
}
extern char* pos_bin2json(void* bytes, int length) {
  return call_bin2json(pos_bin2json_closure, bytes, length);
}

/* --------------------
 * Convert from json to binary strings
 * //FSTODO check for memory leaks in these functions
 * -------------------- */

// out_bytes is an out parameter, it passes a pointer to a memory location. We allocate memory then write the location into which we insert
int call_json2bin(const value* closure, char* json, void** out_bytes) {
  if (Is_exception_result(*closure)) {
    printf("call_json2bin: Closure is exception!\n");
  }
  value v = caml_copy_string(json);
  check_string(v);

  value result = caml_callback(*closure, v);
  int length = caml_string_length(result);
  *out_bytes = copy_string_outside_runtime(result);
  return length;
}
extern int user_fn_json2bin(char* json, void** out_bytes) {
  return call_json2bin(user_fn_json2bin_closure, json, out_bytes);
}
extern int user_tipe_json2bin(char* json, void** out_bytes) {
  return call_json2bin(user_tipe_json2bin_closure, json, out_bytes);
}
extern int handler_json2bin(char* json, void** out_bytes) {
  return call_json2bin(handler_json2bin_closure, json, out_bytes);
}
extern int db_json2bin(char* json, void** out_bytes) {
  return call_json2bin(db_json2bin_closure, json, out_bytes);
}
extern int oplist_json2bin(char* json, void** out_bytes) {
  return call_json2bin(oplist_json2bin_closure, json, out_bytes);
}
extern int pos_json2bin(char* json, void** out_bytes) {
  return call_json2bin(pos_json2bin_closure, json, out_bytes);
}


/* --------------------
 * OCaml values
 * -------------------- */
extern char* digest () {
  return copy_string_outside_runtime(*digest_value);
}