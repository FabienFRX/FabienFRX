const jwt = require("jsonwebtoken");

function authentification(req, res, next) {
    const { authorization } = req.headers;

    if (authorization) {
        const token = authorization.split(" ")[1];
        jwt.verify(token, process.env.SECRET, function(err, decoded) {
            if (err)
                return res.status(498).json({ "msg": "Token is not valid" });
            res.user = decoded.user;
            next();
        });
    } else {
        return res.status(498).json({ "msg": "No token , authorization denied" });
    }
};

module.exports = authentification;