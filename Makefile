install:
	@sudo cp -p app/judgel /usr/local/bin/judgel
	@sudo cp -p app/judgel.out /usr/bin/judgel.out
uninstall:
	@sudo rm -f /usr/local/bin/judgel
	@sudo rm -f /usr/bin/judgel.out
