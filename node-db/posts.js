let db = require("./database.js").db;
let Account = require("./database.js").Account;

module.exports = [
	{
		name: "/addAccount",
		method: (req, res) => {
			let _successful = 1;
			req.body.dateCreated = new Date();
			let account = new Account(req.body);
			account.save((err, account) => {
				if (err) {
					_successful = 0;
					//throw err;
				}
				console.log(`New account '${account.name}' [hash: ${account.password}] added.`);
			});
			res.setHeader("Content-Type", "application/json");
			res.json({
				successful: _successful
			});
		}
	}
];