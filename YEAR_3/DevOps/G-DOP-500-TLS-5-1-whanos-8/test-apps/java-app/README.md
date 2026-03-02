# Whanos Java Test Application

Simple Java application for testing Whanos CI/CD pipeline.

## Build

```bash
cd app
mvn clean package
```

## Run

```bash
java -jar target/java-test-app-1.0-SNAPSHOT.jar
```

## Whanos Deployment

This application is automatically detected by Whanos as a Java project (presence of `app/pom.xml`).
