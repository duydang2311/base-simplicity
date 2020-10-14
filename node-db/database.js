const mongoose = require("mongoose");

let AccountSchema = mongoose.Schema({
	name: String,
	password: String,
	age: Number
});

let Account = mongoose.model("Account", AccountSchema, "accounts");

let db_successful = 0;
mongoose.connect("mongodb://localhost:27017/simplicity", { useNewUrlParser: true, useCreateIndex: true, useFindAndModify: false, useUnifiedTopology: true}, (err) => {
	if(err) {
		throw err;
	}
});

let db = mongoose.connection;

db.on("error", console.error.bind(console, 'Connection Error:'));
db.once("open", function () {
	db_successful = 1;
});

module.exports = {
	isConnectionSuccessful: () => {
		return db_successful;
	},
	db: db,
	Account: Account
};