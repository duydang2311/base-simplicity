let Account = require("./database.js").Account;

module.exports = [
	{
		name: "/databaseConnectionVerify",
		method: (req, res) => {
			console.log(`db connection check ${require("./database.js").isConnectionSuccessful()}`);
			res.json({
				"successful": require("./database.js").isConnectionSuccessful()
			});
		}
	},
	{
		name: "/findAccount",
		method: (req, res) => {
			console.log(req.query);
			Account.findOne({
				name: req.query.name
			}, (err, account) => {
				res.json(account);
				console.log(account);
			});
		}
	}
];