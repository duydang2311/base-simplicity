#include <a_samp>
#include <PawnPlus>
#include <bcrypt>

stock Task:db_sendConnectionCheck() {
	new Task:t = task_new();
	//task_detach();
	//task_yield(t);

	new RequestsClient:req = RequestsClient("http://localhost:3000/");
    RequestJSON(
        req,
        "databaseConnectionVerify",
        HTTP_METHOD_GET,
        "db_connectionVerified",
        .headers = RequestHeaders()
    );

	pawn_register_callback("db_connectionVerified", "tdb_connectionVerified", _, "ei", t); //"e" does not need an argument
	pawn_register_callback("OnRequestFailure", "t_OnRequestFailure", _, "eii", t, req); //"e" does not need an argument
	return Task:t;
}

forward db_connectionVerified(Request:id, E_HTTP_STATUS:status, Node:node);
public db_connectionVerified(Request:id, E_HTTP_STATUS:status, Node:node) {
    new value;
    JsonGetInt(node, "failed", value);
    return 1;
}

forward tdb_connectionVerified(CallbackHandler:cb_id, Task:t, Request:id, E_HTTP_STATUS:status, Node:node);
public tdb_connectionVerified(CallbackHandler:cb_id, Task:t, Request:id, E_HTTP_STATUS:status, Node:node) {
	task_set_result(t, node);
	pawn_unregister_callback(cb_id);
	return 1;
}

public OnRequestFailure(Request:id, errorCode, errorMessage[], len) {
	return 1;
}

forward t_OnRequestFailure(CallbackHandler:cb_id, Task:t, Request:source, Request:id, errorCode, errorMessage[], len);
public t_OnRequestFailure(CallbackHandler:cb_id, Task:t, Request:source, Request:id, errorCode, errorMessage[], len) {
	if(id == source) {
		task_set_result(t, JsonObject("failure", JsonInt(0)));
		pawn_unregister_callback(cb_id);
	}
}

stock Task:db_getAccount(const name[]) {
	new
		Task:t = task_new(),
		string[64],
		RequestsClient:req = RequestsClient("http://localhost:3000/");
	format(string, sizeof(string), "findAccount?name=%s", name);
    RequestJSON(
        req,
        string,
        HTTP_METHOD_GET,
        "db_accountFound",
        .headers = RequestHeaders()
    );

	pawn_register_callback("db_accountFound", "tdb_accountFound", _, "ei", t);
	return Task:t;
}

forward db_accountFound(Request:id, E_HTTP_STATUS:status, Node:node);
public db_accountFound(Request:id, E_HTTP_STATUS:status, Node:node) {
    return 1;
}

forward tdb_accountFound(CallbackHandler:cb_id, Task:t, Request:id, E_HTTP_STATUS:status, Node:node);
public tdb_accountFound(CallbackHandler:cb_id, Task:t, Request:id, E_HTTP_STATUS:status, Node:node) {
	task_set_result(t, node);
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

stock Task:db_bcrypt(password[]) {
	new
		Task:t = task_new();
	bcrypt_hash(password, 12, "db_onPasswordHashed");
	pawn_register_callback("db_onPasswordHashed", "tdb_onPasswordHashed", _, "ei", t);
	return t;
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

forward db_accountAdded(Request:id, E_HTTP_STATUS:status, Node:node);
public db_accountAdded(Request:id, E_HTTP_STATUS:status, Node:node) {
    return 1;
}

forward tdb_accountAdded(CallbackHandler:cb_id, Task:t, Request:id, E_HTTP_STATUS:status, Node:node);
public tdb_accountAdded(CallbackHandler:cb_id, Task:t, Request:id, E_HTTP_STATUS:status, Node:node) {
	task_set_result(t, node);
	pawn_unregister_callback(cb_id);
	return 1;
}