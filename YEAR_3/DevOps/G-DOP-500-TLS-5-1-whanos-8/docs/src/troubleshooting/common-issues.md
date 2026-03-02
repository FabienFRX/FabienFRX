# Common Issues

Solutions to the most frequently encountered problems.

## Quick Diagnostics

Run this script to check system health:

```bash
#!/bin/bash
echo "=== Whanos Health Check ==="

# Check Docker
echo "Docker:"
docker ps --filter "name=whanos" --format "{{.Names}}: {{.Status}}" || echo "❌ Docker error"

# Check k3d
echo -e "\nKubernetes Cluster:"
k3d cluster list || echo "❌ k3d not installed"

# Check registries
echo -e "\nDocker Registries:"
curl -s http://localhost:5000/v2/_catalog | jq . || echo "❌ whanos-registry not accessible"
curl -s http://localhost:5001/v2/_catalog | jq . || echo "❌ k3d-registry not accessible"

# Check Jenkins
echo -e "\nJenkins:"
curl -s http://localhost:8080 > /dev/null && echo "✅ Jenkins accessible" || echo "❌ Jenkins not accessible"

# Check Kubernetes
echo -e "\nKubernetes Nodes:"
export KUBECONFIG=$(k3d kubeconfig write whanos-cluster 2>/dev/null)
kubectl get nodes 2>/dev/null || echo "❌ Cannot connect to cluster"
```

## Jenkins Issues

### Jenkins Container Keeps Crashing

**Symptoms**: `docker ps` shows Jenkins with status `Exited`

**Diagnosis**:
```bash
docker logs whanos-jenkins --tail 50
```

**Solutions**:

1. **Out of Memory**:
   ```bash
   # Increase Docker memory limit
   # Docker Desktop: Settings → Resources → Memory (increase to 4GB+)

   # Restart Jenkins
   docker restart whanos-jenkins
   ```

2. **Port Conflict**:
   ```bash
   # Check what's using port 8080
   lsof -i :8080

   # Kill conflicting process or change Jenkins port
   docker stop whanos-jenkins
   docker rm whanos-jenkins
   # Re-deploy with different port in terraform
   ```

3. **Corrupted Data**:
   ```bash
   # Backup and reset
   docker stop whanos-jenkins
   docker cp whanos-jenkins:/var/jenkins_home ./jenkins-backup
   docker rm whanos-jenkins
   # Re-deploy
   ./scripts/deploy.sh
   ```

### Cannot Access Jenkins UI

**Symptoms**: Browser shows "Connection refused" on http://localhost:8080

**Diagnosis**:
```bash
# Check if container is running
docker ps --filter "name=whanos-jenkins"

# Check logs
docker logs whanos-jenkins | grep "Jenkins is fully up"
```

**Solutions**:

1. **Wait for Startup** (can take 1-2 minutes):
   ```bash
   docker logs -f whanos-jenkins
   # Wait for: "Jenkins is fully up and running"
   ```

2. **Check Port Mapping**:
   ```bash
   docker port whanos-jenkins
   # Should show: 8080/tcp -> 0.0.0.0:8080
   ```

3. **Firewall Issues**:
   ```bash
   # Allow port 8080
   sudo ufw allow 8080
   ```

### Build Fails: "No such image"

**Symptoms**: Pipeline fails with "pull access denied" or "image not found"

**Diagnosis**:
```bash
# Check if base images exist
docker exec whanos-jenkins docker images | grep whanos
```

**Solution**:
```bash
# Build base images
# In Jenkins UI: Whanos base images → Build all base images → Build Now
# Wait for all jobs to complete

# Verify
docker exec whanos-jenkins docker images | grep whanos
# Should show: whanos-c, whanos-java, whanos-javascript, whanos-python, whanos-befunge
```

## Kubernetes Issues

### kubectl: Connection Refused

**Symptoms**: `kubectl get nodes` returns connection error

**Diagnosis**:
```bash
echo $KUBECONFIG
k3d cluster list
```

**Solutions**:

1. **Set Correct Kubeconfig**:
   ```bash
   export KUBECONFIG=$(k3d kubeconfig write whanos-cluster)
   kubectl get nodes
   ```

2. **Cluster Not Running**:
   ```bash
   k3d cluster list
   # If stopped:
   k3d cluster start whanos-cluster
   ```

3. **Wrong Cluster Context**:
   ```bash
   kubectl config get-contexts
   kubectl config use-context k3d-whanos-cluster
   ```

### Pods Stuck in ImagePullBackOff

**Symptoms**: `kubectl get pods -n whanos` shows ImagePullBackOff

**Diagnosis**:
```bash
kubectl describe pod <pod-name> -n whanos
# Look for: "Failed to pull image"
```

**Solutions**:

1. **Wrong Registry URL**:
   Check Jenkinsfile.template has:
   ```groovy
   REGISTRY_URL = "${env.REGISTRY_URL ?: 'k3d-whanos-registry:5000'}"
   ```

2. **Image Not Pushed**:
   ```bash
   # Check registry contents
   curl http://localhost:5001/v2/_catalog

   # Rebuild and push
   # Trigger Jenkins build again
   ```

3. **Registry Not Connected to Cluster**:
   ```bash
   # Verify k3d cluster uses registry
   k3d cluster list -o json | grep registry
   ```

### Deployment Not Created

**Symptoms**: After successful build, no deployment in Kubernetes

**Diagnosis**:
```bash
kubectl get deployments -n whanos
kubectl get pods -n whanos
```

**Solutions**:

1. **Missing whanos.yml**:
   ```bash
   # Check if whanos.yml exists in repo
   # Add whanos.yml:
   cat > whanos.yml <<EOF
   deployment:
     replicas: 1
     ports:
       - 5000
   EOF
   ```

2. **Invalid whanos.yml**:
   ```bash
   # Validate YAML syntax
   python3 -c "import yaml; yaml.safe_load(open('whanos.yml'))"
   ```

3. **Namespace Doesn't Exist**:
   ```bash
   kubectl create namespace whanos
   ```

## Docker Issues

### Registry Not Accessible

**Symptoms**: `curl http://localhost:5000/v2/_catalog` fails

**Diagnosis**:
```bash
docker ps --filter "name=registry"
```

**Solutions**:

1. **Container Not Running**:
   ```bash
   docker start whanos-registry
   docker start k3d-whanos-registry
   ```

2. **Port Conflict**:
   ```bash
   lsof -i :5000
   lsof -i :5001
   # Stop conflicting services
   ```

### "Cannot connect to Docker daemon"

**Symptoms**: Docker commands fail with daemon connection error

**Solutions**:

1. **Start Docker**:
   ```bash
   # Linux
   sudo systemctl start docker

   # macOS/Windows
   # Start Docker Desktop
   ```

2. **Permission Denied**:
   ```bash
   sudo usermod -aG docker $USER
   newgrp docker
   ```

3. **WSL2 Issues** (Windows):
   ```bash
   wsl --shutdown
   # Restart WSL
   ```

## Network Issues

### Jenkins Cannot Connect to k3d Cluster

**Symptoms**: Pipeline fails at Kubernetes deployment with connection error

**Diagnosis**:
```bash
docker network inspect k3d-whanos-cluster | grep whanos-jenkins
```

**Solution**:
```bash
# Connect Jenkins to k3d network
docker network connect k3d-whanos-cluster whanos-jenkins

# Restart Jenkins
docker restart whanos-jenkins
```

### Cannot Access Deployed Application

**Symptoms**: Application deployed but not accessible from browser

**Diagnosis**:
```bash
kubectl get svc -n whanos
kubectl get pods -n whanos
```

**Solutions**:

1. **Service Not Created**:
   Add ports to whanos.yml:
   ```yaml
   deployment:
     ports:
       - 5000
   ```

2. **Access via LoadBalancer**:
   ```bash
   # Applications are exposed on port 8081
   curl http://localhost:8081
   ```

3. **Port Forward for Testing**:
   ```bash
   kubectl port-forward -n whanos <pod-name> 8080:5000
   curl http://localhost:8080
   ```

## Build Issues

### Language Not Detected

**Symptoms**: Pipeline fails with "No supported language detected"

**Diagnosis**:
Check for language marker files:

```bash
# C: Makefile at root
ls Makefile

# Java: pom.xml in app/
ls app/pom.xml

# JavaScript: package.json at root
ls package.json

# Python: requirements.txt at root
ls requirements.txt

# Befunge: main.bf in app/
ls app/main.bf
```

**Solution**: Add the appropriate detection file

### Compilation Fails

**Symptoms**: Build fails during compilation step

**Solutions by Language**:

**C**:
```bash
# Test locally
make
./compiled-app

# Check Makefile output
# Must create 'compiled-app' at root
```

**Java**:
```bash
cd app
mvn clean package
# Check for target/app.jar
```

**JavaScript**:
```bash
npm install
node app
```

**Python**:
```bash
pip install -r requirements.txt
python -m app
```

## Terraform Issues

### Terraform State Locked

**Symptoms**: `terraform apply` fails with state lock error

**Solution**:
```bash
cd terraform/environments/dev
terraform force-unlock <lock-id>
```

### Terraform Apply Fails

**Symptoms**: Infrastructure deployment fails

**Solutions**:

1. **Destroy and Recreate**:
   ```bash
   terraform destroy
   terraform apply
   ```

2. **Check Docker**:
   ```bash
   docker ps
   # Ensure Docker is running
   ```

## Getting Help

If issues persist:

1. **Check Logs**:
   ```bash
   # Jenkins
   docker logs whanos-jenkins --tail 100

   # Kubernetes
   kubectl logs -n whanos <pod-name>

   # k3d
   k3d cluster list
   docker logs k3d-whanos-cluster-server-0
   ```

2. **Restart Everything**:
   ```bash
   ./scripts/destroy.sh
   ./scripts/deploy.sh
   ```

3. **Clean Install**:
   ```bash
   # Remove all Whanos resources
   docker stop $(docker ps -a --filter "name=whanos" -q)
   docker rm $(docker ps -a --filter "name=whanos" -q)
   k3d cluster delete whanos-cluster
   k3d registry delete whanos-registry

   # Fresh install
   ./scripts/deploy.sh
   ```

## See Also

- [Jenkins Problems](./jenkins.md)
- [Docker Issues](./docker.md)
- [Kubernetes Issues](./kubernetes.md)
- [Network Problems](./network.md)
