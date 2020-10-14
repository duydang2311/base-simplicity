let db = require("./database.js").db;
let Account = require("./database.js").Account;

module.exports = [
	{
		name: "/addAccount",
		method: (req, res) => {
			let _successful = 0;
			console.log(req.body);

			let account = new Account(req.body);
			account.save((err, account) => {
				if(err) {
					throw err;
				}
				console.log(`New account '${account.name}' [age: ${account.age} | password: ${account.password}] added.`);
			});

			_successful = 1;
			res.json({
				successful: _successful
			});
		}
	}
];