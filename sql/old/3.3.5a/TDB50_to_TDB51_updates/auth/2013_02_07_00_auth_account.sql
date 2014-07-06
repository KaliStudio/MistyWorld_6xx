UPDATE `account` SET `sessionkey`='';
ALTER TABLE `account`
CHANGE `sessionkey` `sessionkey` varchar(80) NOT NULL DEFAULT '' COMMENT 'Temporary storage of session key used to pass data from authserveur to Mondial' AFTER `sha_pass_hash`;
