let Account = require("./database.js").Account;
let isConnectionSuccessful = require("./database.js").isConnectionSuccessful;

module.exports = [
	{
		name: "/databaseConnectionVerify",
		method: (req, res) => {
			res.json({
				"successful": isConnectionSuccessful()
			});
		}
	},
	{
		name: "/retrieveAccount",
		method: (req, res) => {
			Account.findOne({
				name: req.query.name
			}, (err, account) => {
				res.json(account);
			});
		}
	},
	{
		name: "/authenticateAccount",
		method: (req, res) => {
			Account.findOne({
				name: req.query.name,
				password: req.query.hash
			}, (err, account) => {
				res.json(account);
			});
		}
	}
];