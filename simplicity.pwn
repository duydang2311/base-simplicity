/*
    ༼ つ ◕◕ ༽つ༼ つ ◕◕ ༽つ༼ つ ◕◕ ༽つ༼ つ ◕◕ ༽つ༼ つ ◕◕ ༽つ༼ つ ◕◕ ༽つ༼ つ ◕◕ ༽つ
    ----------------------------------------------------------------------
    |                                                                    |
    |                           BASE SIMPLICITY                          |
    |                                                                    |
    |           SIMPLE BASE USING PAWN ALONG WITH NODEJS + MONGODB       |
    |                                                                    |
    ----------------------------------------------------------------------
*/

#define PP_SYNTAX_AWAIT

#include <a_samp>
#include <requests>
#include <PawnPlus>
#include <bcrypt>

#include <database>

main() {

}

stock randomCharacter() {
    return (random(2)) ? random('z' - 'a') + 'a' : random('Z' - 'A') + 'A';
}

public OnGameModeInit() {
    /*
        ----------------------------------------------------------------------
        MongoDB connection verification
        ----------------------------------------------------------------------
    */
    task_yield(1);
    new Node:res = await db_sendConnectionCheck();

    /*
        Node:res is a JSON returned by db_sendConnectionCheck
        res.successful == 1 : successful
        res.successful == 0 : failed
    */
    new successful = 1,
        json_ret;
    json_ret = JsonGetInt(res, "successful", successful);
    if(!successful || json_ret) {
        print("ERROR: failed to connect to MongoDB\n\n");
        SendRconCommand("exit");
        return -1;
    } else {
        print("INFO: MongoDB connected\n\n");
    }

    /*
        ----------------------------------------------------------------------
        Sending a request to add a new account
        ----------------------------------------------------------------------
    */

    new acc_name[64];
    format(acc_name, sizeof(acc_name), "duydang-%c%c%c%c%c%c%c%c", randomCharacter(), randomCharacter(), randomCharacter(), randomCharacter(), randomCharacter(), randomCharacter(), randomCharacter(), randomCharacter());
    new Node:account = JsonObject(
        "name", JsonString(acc_name),
        "age", JsonInt(random(13) + 18),
        "password", JsonString("this-is-raw-password")
    );
    res = await db_addAccount(account);

    json_ret = JsonGetInt(res, "successful", successful);
    print((successful && !json_ret) ? "INFO: A new account has been added" : "ERROR: Failed to add a new account");

    /*
        ----------------------------------------------------------------------
        Sending a request to find an account by name
        ----------------------------------------------------------------------
    */

    new
        name[64],
        hash[BCRYPT_HASH_LENGTH],
        age;

    res = await db_getAccount(acc_name);

    json_ret = JsonGetString(res, "name", name, sizeof(name));
    json_ret = JsonGetInt(res, "age", age);
    json_ret = JsonGetString(res, "password", hash, sizeof(hash));

    if(json_ret) {
        print("\nFailed to find the account.");
    } else {
        printf("\nAccount found: name: %s, age: %d, hash: %s", name, age, hash);
    }
    return 1;
}