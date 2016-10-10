Maven bootstrap example

just run mvn scm:bootstrap

By the way, maven-scm-plugin 1.9.5 has a nasty bug, where it throws a NullPointerException.
If you are using 1.9.5 a possible work around is to pass in -DmavenHome=""
