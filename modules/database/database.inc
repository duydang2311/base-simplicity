#if defined _inc_guard_database
    #endinput
#endif

#define _inc_guard_database

#include <a_samp>
#include <PawnPlus>
#include <bcrypt>
#include <requests>

stock Task:db_sendConnectionCheck() {
    new Task:t = task_new();

    new RequestsClient:req = RequestsClient("http://localhost:3000/");
    RequestJSON(
        req,
        "databaseConnectionVerify",
        HTTP_METHOD_GET,
        "db_connectionVerified",
        .headers = RequestHeaders()
    );

    pawn_register_callback("db_connectionVerified", "tdb_connectionVerified", _, "ei", t); //"e" does not need an argument
    pawn_register_callback("OnRequestFailure", "t_OnRequestFailure", _, "eii", t, _:req); //"e" does not need an argument
    return Task:t;
}

stock Task:db_retrieveAccount(const name[]) {
    new
        Task:t = task_new(),
        string[64],
        RequestsClient:req = RequestsClient("http://localhost:3000/");
    format(string, sizeof(string), "retrieveAccount?name=%s", name);
    RequestJSON(
        req,
        string,
        HTTP_METHOD_GET,
        "db_accountRetrieved",
        .headers = RequestHeaders()
    );

    pawn_register_callback("db_accountRetrieved", "tdb_accountRetrieved", _, "ei", t);
    return Task:t;
}

stock Task:db_bcrypt(password[]) {
    new
        Task:t = task_new();
    bcrypt_hash(password, 12, "db_onPasswordHashed");
    pawn_register_callback("db_onPasswordHashed", "tdb_onPasswordHashed", _, "ei", t);
    return t;
}

stock Task:db_bcryptVerify(hash[], input[]) {
    new Task:t = task_new();

    bcrypt_check(input, hash, "db_onPasswordVerified");
    pawn_register_callback("db_onPasswordVerified", "tdb_onPasswordVerified", _, "ei", t);
    return t;
}

forward db_onPasswordVerified();
public db_onPasswordVerified() {
    return 1;
}

forward tdb_onPasswordVerified(CallbackHandler:cb_id, Task:t);
public tdb_onPasswordVerified(CallbackHandler:cb_id, Task:t) {
    printf("%d", bcrypt_is_equal());
    task_set_result(t, bcrypt_is_equal());
    pawn_unregister_callback(cb_id);
    return 1;
}

stock Task:db_addAccount(Node:data) {
    new
    Task:t = task_new(),
        hash[BCRYPT_HASH_LENGTH],
        password[128],
        RequestsClient:req = RequestsClient("http://localhost:3000/");

    JsonGetString(data, "password", password, sizeof(password));
    await_arr(hash) db_bcrypt(password);

    JsonSetString(data, "password", hash, sizeof(hash));
    JsonGetString(data, "password", password, sizeof(password));
    RequestJSON(
        req,
        "addAccount",
        HTTP_METHOD_POST,
        "db_accountAdded",
        data,
        .headers = RequestHeaders()
    );

    pawn_register_callback("db_accountAdded", "tdb_accountAdded", _, "ei", t);
    return t;
}

stock Task:db_sendAccountAuth(const name[], const hash[]) {
    new
        Task:t = task_new(),
        string[512];

    format(string, sizeof(string), "authenticateAccount?name=%s&hash=%s", name, hash);
    RequestJSON(
        RequestsClient("http://localhost:3000"),
        string,
        HTTP_METHOD_GET,
        "db_accountAuthenticated",
        .headers = RequestHeaders()
    );

    pawn_register_callback("db_accountAuthenticated", "tdb_accountAuthenticated", _, "ei", t);
    return t;
}

forward db_accountAuthenticated(Request:id, E_HTTP_STATUS:status, Node:node);
public db_accountAuthenticated(Request:id, E_HTTP_STATUS:status, Node:node) {
    return 1;
}

forward tdb_accountAuthenticated(CallbackHandler:cb_id, Task:t, Request:id, E_HTTP_STATUS:status, Node:node);
public tdb_accountAuthenticated(CallbackHandler:cb_id, Task:t, Request:id, E_HTTP_STATUS:status, Node:node) {
    task_set_result(t, _:node);
    pawn_unregister_callback(cb_id);
    return 1;
}

forward db_connectionVerified(Request:id, E_HTTP_STATUS:status, Node:node);
public db_connectionVerified(Request:id, E_HTTP_STATUS:status, Node:node) {
    new value;
    JsonGetInt(node, "failed", value);
    return 1;
}

forward tdb_connectionVerified(CallbackHandler:cb_id, Task:t, Request:id, E_HTTP_STATUS:status, Node:node);
public tdb_connectionVerified(CallbackHandler:cb_id, Task:t, Request:id, E_HTTP_STATUS:status, Node:node) {
    task_set_result(t, _:node);
    pawn_unregister_callback(cb_id);
    return 1;
}

forward db_onPasswordHashed();
public db_onPasswordHashed() { return 1; }

forward tdb_onPasswordHashed(CallbackHandler:cb_id, Task:t);
public tdb_onPasswordHashed(CallbackHandler:cb_id, Task:t) {
    new hash[BCRYPT_HASH_LENGTH];
    bcrypt_get_hash(hash);
    task_set_result_arr(t, hash, BCRYPT_HASH_LENGTH);
    pawn_unregister_callback(cb_id);
    return 1;
}

forward db_accountRetrieved(Request:id, E_HTTP_STATUS:status, Node:node);
public db_accountRetrieved(Request:id, E_HTTP_STATUS:status, Node:node) {
    return 1;
}

forward tdb_accountRetrieved(CallbackHandler:cb_id, Task:t, Request:id, E_HTTP_STATUS:status, Node:node);
public tdb_accountRetrieved(CallbackHandler:cb_id, Task:t, Request:id, E_HTTP_STATUS:status, Node:node) {
    task_set_result(t, _:node);
    pawn_unregister_callback(cb_id);
    return 1;
}

forward db_accountAdded(Request:id, E_HTTP_STATUS:status, Node:node);
public db_accountAdded(Request:id, E_HTTP_STATUS:status, Node:node) {
    return 1;
}

forward tdb_accountAdded(CallbackHandler:cb_id, Task:t, Request:id, E_HTTP_STATUS:status, Node:node);
public tdb_accountAdded(CallbackHandler:cb_id, Task:t, Request:id, E_HTTP_STATUS:status, Node:node) {
    task_set_result(t, _:node);
    pawn_unregister_callback(cb_id);
    return 1;
}

forward t_OnRequestFailure(CallbackHandler:cb_id, Task:t, Request:source, Request:id, errorCode, errorMessage[], len);
public t_OnRequestFailure(CallbackHandler:cb_id, Task:t, Request:source, Request:id, errorCode, errorMessage[], len) {
    if(id == source) {
        task_set_result(t, _:JsonObject("failure", JsonInt(0)));
        pawn_unregister_callback(cb_id);
    }
}

#include <YSI_Coding\y_hooks>

hook OnGameModeInit() {
    task_yield(1);
    new Node:res = Node:await db_sendConnectionCheck();

    new successful = 1,
        json_ret = JsonGetInt(res, "successful", successful);

    if(!successful || json_ret) {
        print("ERROR: failed to connect to MongoDB\n\n");
        SendRconCommand("exit");
        return -1;
    } else {
        print("INFO: MongoDB connected");
    }
    print("INFO: modules/database loaded");
    return 1;
}