DELETE FROM `command` WHERE `name` LIKE 'serveur togglequerylog';
INSERT INTO `command` (`name`,`security`,`help`) VALUES
('serveur togglequerylog',4,'Toggle SQL driver query logging.');
