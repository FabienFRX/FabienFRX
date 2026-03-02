# Jenkins Configuration for Whanos

This directory contains the Jenkins configuration for the Whanos CI/CD infrastructure.

## Structure

```
jenkins/
├── config/
│   ├── jenkins.yaml          # Jenkins Configuration as Code (JCasC)
│   └── job-dsl/              # Job DSL scripts (if needed)
├── scripts/
│   └── build-and-deploy.sh   # Legacy build script
├── Jenkinsfile               # Main pipeline for projects
├── Jenkinsfile.template      # Template for reference
├── Jenkinsfile-local         # Local testing pipeline
└── README.md                 # This file
```

## Jenkins Configuration (jenkins.yaml)

The `jenkins.yaml` file uses Jenkins Configuration as Code (JCasC) to automatically configure:

### 1. Users
- **Admin** user with `admin` / `admin` credentials
- Sign-up is disabled for security

### 2. Folders
- **Whanos base images** - Contains build jobs for all base Docker images
- **Projects** - Contains dynamically created project jobs

### 3. Jobs

#### Base Image Build Jobs
Located in `Whanos base images/` folder:
- `whanos-c` - Builds the C base image
- `whanos-java` - Builds the Java base image
- `whanos-javascript` - Builds the JavaScript base image
- `whanos-python` - Builds the Python base image
- `whanos-befunge` - Builds the Befunge base image
- `Build all base images` - Triggers all base image builds in parallel

#### Project Linking Job
Located at root level:
- `link-project` - Creates a new project job in the Projects folder

## How to Use

### 1. Build Base Images

After Jenkins starts, you need to build the base images first:

1. Go to `Whanos base images/Build all base images`
2. Click "Build Now"
3. Wait for all base images to be built (takes a few minutes)

Alternatively, build individual images:
- Navigate to `Whanos base images/whanos-python` (or any language)
- Click "Build Now"

### 2. Link a Project

To link a Git repository to Whanos:

1. Go to the `link-project` job at root level
2. Click "Build with Parameters"
3. Fill in the parameters:
   - **REPOSITORY_URL**: Git repository URL (e.g., `https://github.com/user/repo.git`)
   - **PROJECT_NAME**: Name for the project (will be used as job name)
   - **GIT_CREDENTIALS_ID**: (Optional) Jenkins credentials ID for private repos
   - **DEFAULT_BRANCH**: Branch to monitor (default: `main`)
4. Click "Build"

This will create a new job in the `Projects/` folder.

### 3. Project Job Behavior

Once a project is linked, its job will:

1. **Check for changes every minute** (polls the repository)
2. **On change detection**:
   - Clone the repository
   - Detect the language (based on marker files)
   - Build a Docker image using either:
     - Custom `Dockerfile` if present in the repo
     - Whanos standalone image for the detected language
   - Push the image to the local Docker registry
   - If `whanos.yml` exists with a `deployment` section:
     - Deploy the application to Kubernetes
     - Apply replicas, ports, and resource configurations

## Supported Languages

The system auto-detects languages based on these files:

| Language   | Detection File      | Base Image Name    |
|------------|--------------------|--------------------|
| Python     | `requirements.txt` | `whanos-python`    |
| JavaScript | `package.json`     | `whanos-javascript`|
| Java       | `app/pom.xml`      | `whanos-java`      |
| C          | `Makefile`         | `whanos-c`         |
| Befunge    | `app/main.bf`      | `whanos-befunge`   |

## whanos.yml Configuration

Projects can include a `whanos.yml` file at the root to configure deployment:

```yaml
deployment:
  replicas: 2
  ports:
    - 8080
    - 9090
  resources:
    requests:
      memory: "128Mi"
      cpu: "100m"
    limits:
      memory: "256Mi"
      cpu: "200m"
```

### Configuration Options

- **replicas**: Number of pod replicas (default: 1)
- **ports**: List of container ports to expose
- **resources**: Kubernetes resource requests and limits

If no `whanos.yml` file is present, or if it doesn't have a `deployment` section, the image will be built but not deployed.

## Jenkinsfile

Projects can optionally include their own `Jenkinsfile` for custom pipeline logic. If not present, they'll use the standard Whanos pipeline defined in this directory.

## Updating Configuration

After modifying `jenkins.yaml`:

1. Restart Jenkins, OR
2. Use "Manage Jenkins" → "Configuration as Code" → "Reload existing configuration"

## Troubleshooting

### Jobs not appearing
- Check Jenkins logs for JCasC errors
- Ensure Job DSL plugin is installed
- Reload configuration from Configuration as Code page

### Base images failing to build
- Ensure `/var/jenkins_home/whanos-images` is mounted correctly
- Check that Dockerfiles exist in the images directory
- Verify Docker socket is accessible (`/var/run/docker.sock`)

### Projects not deploying
- Ensure kubeconfig is mounted at `/root/.kube/config`
- Verify kubectl can access the cluster
- Check that the `whanos` namespace exists in Kubernetes
- Verify whanos.yml syntax

### Private repositories not working
1. Add credentials in Jenkins: "Manage Jenkins" → "Credentials"
2. Use the credentials ID in the `GIT_CREDENTIALS_ID` parameter
3. Supported types: Username/Password, SSH key

## Environment Variables

Jobs use these environment variables:

- `REGISTRY_URL`: Docker registry URL (default: `localhost:5000`)
- `K3D_CLUSTER_NAME`: Kubernetes cluster name (default: `whanos-cluster`)
- `APP_NAME`: Application name (extracted from job name)
- `IMAGE_TAG`: Build number (unique per build)

## Next Steps

1. Build all base images
2. Link your first project
3. Check the `Projects/` folder for your linked project
4. Monitor builds and deployments

For more information, see the main Whanos documentation in the `docs/` directory.
