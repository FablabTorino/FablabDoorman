#!/usr/bin/env python

from sys import argv
import sqlite3
from datetime import datetime
import os
source_dir = os.path.dirname(os.path.abspath(__file__))
cardcode = ''.join(argv[1])

def insert_user(username=None, cardcode=None):
	db = sqlite3.connect(os.path.join(source_dir, 'logger.db'))
	c = db.cursor()
	log = open('log.txt', 'w')
	if username and cardcode:
		c.execute("INSERT INTO allowedusers(username, cardcode) VALUES (?,?)", (username, cardcode))
		db.commit()
	else:
		log.write('You need to provide a username and a cardcode')
		return
	log.close()

	return

db = sqlite3.connect(os.path.join(source_dir, 'logger.db'))
c = db.cursor()

c.execute('''CREATE TABLE IF NOT EXISTS intothedoor(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, datetime TEXT NOT NULL, cardcode TEXT NOT NULL)''')
c.execute('''CREATE TABLE IF NOT EXISTS allowedusers(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, username TEXT NOT NULL, cardcode TEXT NOT NULL)''')


#insert_user('Federico Vanzati', '1815453204')

def log_rfid_read(cardcode):
	db = sqlite3.connect(os.path.join(source_dir, 'logger.db'))
	c = db.cursor()
	date = str(datetime.utcnow().isoformat())
	c.execute("INSERT INTO intothedoor(datetime, cardcode) VALUES (?,?)", (date, cardcode))
	db.commit()
	return

def check_allowed_user(cardcode=None):
	db = sqlite3.connect(os.path.join(source_dir, 'logger.db'))
	c = db.cursor()
	if cardcode:
		cardcode = (cardcode,)
		c.execute("select count (*) from allowedusers where cardcode=?", cardcode)
		result = c.fetchone()[0]
		
		if result > 0:
			print 'y'
			return
		else:
			print 'n'

	return

log_rfid_read(cardcode)
check_allowed_user(cardcode)
