# Creating a Whanos-Compatible Repository

Learn how to structure your repository to work seamlessly with Whanos.

## Repository Structure

A Whanos-compatible repository must follow this structure:

```
your-repository/
├── app/                    # Your application code (REQUIRED)
│   ├── src/               # Source files
│   ├── __main__.py        # Entry point (Python example)
│   └── ...
├── Makefile               # For C projects
├── pom.xml                # For Java projects (in app/)
├── package.json           # For JavaScript projects
├── requirements.txt       # For Python projects
├── Dockerfile             # OPTIONAL: Custom Dockerfile
├── whanos.yml             # OPTIONAL: Kubernetes configuration
└── README.md              # Your project documentation
```

## Language-Specific Requirements

### C Projects

**Detection Criteria**: Presence of `Makefile` at repository root

**Required Files**:
- `Makefile` at root
- `app/` directory with source code

**Build Expectations**:
```bash
make                    # Should compile
./compiled-app          # Should be the output binary
```

**Example Structure**:
```
c-project/
├── Makefile
└── app/
    ├── main.c
    ├── utils.c
    └── utils.h
```

**Makefile Template**:
```makefile
CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRC = app/main.c app/utils.c
OBJ = $(SRC:.c=.o)

compiled-app: $(OBJ)
	$(CC) $(OBJ) -o compiled-app

clean:
	rm -f $(OBJ) compiled-app
```

### Java Projects

**Detection Criteria**: Presence of `app/pom.xml`

**Required Files**:
- `app/pom.xml`
- `app/src/` with Java source code

**Build Expectations**:
```bash
cd app
mvn package
# Output: target/app.jar
```

**Example Structure**:
```
java-project/
└── app/
    ├── pom.xml
    └── src/
        └── main/
            └── java/
                └── com/
                    └── example/
                        └── Main.java
```

**pom.xml Template**:
```xml
<project>
    <modelVersion>4.0.0</modelVersion>
    <groupId>com.example</groupId>
    <artifactId>app</artifactId>
    <version>1.0.0</version>

    <properties>
        <maven.compiler.source>21</maven.compiler.source>
        <maven.compiler.target>21</maven.compiler.target>
    </properties>

    <build>
        <finalName>app</finalName>
    </build>
</project>
```

### JavaScript Projects

**Detection Criteria**: Presence of `package.json` at root

**Required Files**:
- `package.json` at root
- `app/` directory with JS code

**Build Expectations**:
```bash
npm ci --only=production
node app
```

**Example Structure**:
```
javascript-project/
├── package.json
├── package-lock.json
└── app/
    ├── index.js
    └── server.js
```

**package.json Template**:
```json
{
  "name": "my-app",
  "version": "1.0.0",
  "main": "app/index.js",
  "scripts": {
    "start": "node app"
  },
  "dependencies": {
    "express": "^4.18.0"
  }
}
```

### Python Projects

**Detection Criteria**: Presence of `requirements.txt` at root

**Required Files**:
- `requirements.txt` at root
- `app/` directory as Python module
- `app/__main__.py` as entry point

**Build Expectations**:
```bash
pip install -r requirements.txt
python -m app
```

**Example Structure**:
```
python-project/
├── requirements.txt
└── app/
    ├── __init__.py
    ├── __main__.py
    └── server.py
```

**requirements.txt Template**:
```
Flask==3.0.0
Werkzeug==3.0.1
```

**app/__main__.py Template**:
```python
from app.server import create_app

if __name__ == '__main__':
    app = create_app()
    app.run(host='0.0.0.0', port=5000)
```

### Befunge Projects

**Detection Criteria**: Presence of `app/main.bf`

**Required Files**:
- `app/main.bf`

**Example Structure**:
```
befunge-project/
└── app/
    └── main.bf
```

## Optional Files

### Custom Dockerfile

If you need custom dependencies or build steps:

```dockerfile
FROM whanos-python:latest
# ONBUILD instructions from base image will run here

# Add custom steps
RUN pip install additional-package
ENV CUSTOM_VAR=value
CMD ["python", "-m", "app"]
```

**Important**: Must start with `FROM whanos-<language>:latest`

### whanos.yml

Configure Kubernetes deployment:

```yaml
deployment:
  replicas: 2
  resources:
    requests:
      memory: "128Mi"
      cpu: "250m"
    limits:
      memory: "256Mi"
      cpu: "500m"
  ports:
    - 5000
    - 8080
```

See [whanos.yml Configuration](../kubernetes/whanos-yml.md) for details.

## Validation Checklist

Before linking your project to Whanos:

- [ ] Application code is in `app/` directory
- [ ] Language detection file present (Makefile, pom.xml, package.json, requirements.txt, or main.bf)
- [ ] Application builds successfully locally
- [ ] Application runs successfully locally
- [ ] (Optional) Custom Dockerfile uses whanos base image
- [ ] (Optional) whanos.yml is valid YAML

## Testing Locally

### Test Build

```bash
# For Python
cd your-repo
docker build -f /path/to/whanos/images/python/Dockerfile.standalone -t test .

# Run
docker run --rm test
```

### Test with Base Image

```bash
# Build base image first
docker build -t whanos-python - < /path/to/whanos/images/python/Dockerfile.base

# Create test Dockerfile
cat > Dockerfile.test <<EOF
FROM whanos-python:latest
CMD ["python", "-m", "app"]
EOF

# Build and run
docker build -t test -f Dockerfile.test .
docker run --rm test
```

## Common Mistakes

### ❌ App code not in app/ directory

```
wrong-repo/
├── src/              # ❌ Should be app/
│   └── main.py
└── requirements.txt
```

### ❌ Detection file in wrong location

```
wrong-repo/
├── app/
│   ├── pom.xml      # ❌ Should be app/pom.xml for Java
│   └── src/
└── Makefile         # ❌ C projects need app/ directory too
```

### ❌ Wrong compiled output name (C)

```makefile
# ❌ Wrong
output: main.c
	gcc main.c -o myapp

# ✅ Correct
compiled-app: main.c
	gcc main.c -o compiled-app
```

### ❌ Java output not app.jar

```xml
<!-- ❌ Wrong -->
<build>
    <finalName>myapp</finalName>
</build>

<!-- ✅ Correct -->
<build>
    <finalName>app</finalName>
</build>
```

## Example Repositories

Working examples are provided in the `test-apps/` directory:

- `test-apps/c-app/`
- `test-apps/java-app/`
- `test-apps/javascript-app/`
- `test-apps/python-app/`
- `test-apps/befunge-app/`

## Next Steps

Once your repository is compatible:
1. [Link the Project](./linking-project.md) to Whanos
2. [Configure Deployment](./configuring-deployment.md) with whanos.yml
3. [Monitor the Build](./monitoring.md) in Jenkins
