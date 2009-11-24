#include <node.h>
#include <node_events.h>
#include <assert.h>
#include <string.h>
#include <tcrdb.h>

using namespace v8;
using namespace node;

class TokyoTyrant : public EventEmitter{
  public:
    TCRDB *rdb;
    static void Initialize (v8::Handle<v8::Object> target){
      HandleScope scope;

      Local<FunctionTemplate> t = FunctionTemplate::New(New);

      t->Inherit(EventEmitter::constructor_template);
      t->InstanceTemplate()->SetInternalFieldCount(1);

      NODE_SET_PROTOTYPE_METHOD(t, "init", TokyoTyrantInit);
      NODE_SET_PROTOTYPE_METHOD(t, "vanish", TokyoTyrantVanish);
      NODE_SET_PROTOTYPE_METHOD(t, "out", TokyoTyrantOut);
      NODE_SET_PROTOTYPE_METHOD(t, "put", TokyoTyrantPut);
      NODE_SET_PROTOTYPE_METHOD(t, "get", TokyoTyrantGet);

      target->Set(String::NewSymbol("TokyoTyrant"), t->GetFunction());
    }

    bool TokyoTyrantInit(char *host, int port){
      rdb = tcrdbnew();
      initialized = tcrdbopen(rdb, host, port);
      return initialized;
    }

    void TokyoTyrantException(){
      int error_code;
      const char *message;
      error_code = tcrdbecode(rdb);
      message = tcrdberrmsg(error_code);
      Local<Value> exception = Exception::Error(String::New(message));
      ThrowException(exception);
    }

    void TokyoTyrantFree(){
      tcrdbdel(rdb);
    }

    bool TokyoTyrantVanish(){
      return tcrdbvanish(rdb);
    }

    bool TokyoTyrantOut(char *key){
      return tcrdbout2(rdb, key);
    }

    bool TokyoTyrantPut(char *key, char *value){
      return tcrdbput2(rdb, key, value);
    }

    char *TokyoTyrantGet(char *key){
      char *value;
      int value_size, error_code;

      if(!(value = tcrdbget2(rdb, key))){
        if ((error_code = tcrdbecode(rdb))) TokyoTyrantException();
        return "";
      } else {
        return value;
      }
    }

  protected:
    static Handle<Value> New (const Arguments& args){
      HandleScope scope;

      TokyoTyrant *tokyo_tyrant = new TokyoTyrant();
      tokyo_tyrant->Wrap(args.This());
      return args.This();
    }

    static Handle<Value> TokyoTyrantInit(const Arguments& args){
      TokyoTyrant *tokyo_tyrant = ObjectWrap::Unwrap<TokyoTyrant>(args.This());

      HandleScope scope;

      if (args.Length() < 2 || !args[0]->IsString() || !args[1]->IsNumber()) {
        return ThrowException(String::New("Must include host(string) and port(int) connection arguments"));
      }

      String::AsciiValue host(args[0]->ToString());

      bool r = tokyo_tyrant->TokyoTyrantInit(*host, args[1]->ToNumber()->Value());

      if (!r) tokyo_tyrant->TokyoTyrantException();

      return args.This();
    }

    static Handle<Value> TokyoTyrantVanish(const Arguments& args){
      TokyoTyrant *tokyo_tyrant = ObjectWrap::Unwrap<TokyoTyrant>(args.This());

      HandleScope scope;

      bool r = tokyo_tyrant->TokyoTyrantVanish();
      if (!r) tokyo_tyrant->TokyoTyrantException();

      return args.This();
    }

    static Handle<Value> TokyoTyrantOut(const Arguments& args){
      TokyoTyrant *tokyo_tyrant = ObjectWrap::Unwrap<TokyoTyrant>(args.This());

      HandleScope scope;

      if (args.Length() != 1 || !args[0]->IsString()) {
        return ThrowException(String::New("Must include key(string) argument"));
      }

      String::AsciiValue key(args[0]->ToString());

      bool r = tokyo_tyrant->TokyoTyrantOut(*key);

      if (!r) tokyo_tyrant->TokyoTyrantException();

      return args.This();
    }

    static Handle<Value> TokyoTyrantPut(const Arguments& args){
      TokyoTyrant *tokyo_tyrant = ObjectWrap::Unwrap<TokyoTyrant>(args.This());

      HandleScope scope;

      if (args.Length() < 2 || !args[0]->IsString() || !args[1]->IsString()) {
        return ThrowException(String::New("Must include key(string) and value(string) arguments"));
      }

      String::AsciiValue key(args[0]->ToString());
      String::AsciiValue value(args[1]->ToString());

      bool r = tokyo_tyrant->TokyoTyrantPut(*key, *value);

      if (!r) tokyo_tyrant->TokyoTyrantException();

      return args.This();
    }

    static Handle<Value> TokyoTyrantGet(const Arguments& args){
      char *value;
      TokyoTyrant *tokyo_tyrant = ObjectWrap::Unwrap<TokyoTyrant>(args.This());

      HandleScope scope;

      if (args.Length() != 1 || !args[0]->IsString()) {
        return ThrowException(String::New("Must include key(string) argument"));
      }

      String::AsciiValue key(args[0]->ToString());

      value = tokyo_tyrant->TokyoTyrantGet(*key);

      return String::New(value);
    }

    TokyoTyrant() : EventEmitter(){
      initialized = false;
    }

    ~TokyoTyrant(){
    }

  private:
    bool initialized;
};

extern "C" void
init (Handle<Object> target){
  HandleScope scope;

  TokyoTyrant::Initialize(target);
};
