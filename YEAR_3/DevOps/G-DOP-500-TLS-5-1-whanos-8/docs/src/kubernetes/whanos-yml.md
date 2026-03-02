# whanos.yml Configuration File

The `whanos.yml` file is an optional configuration file that controls how your application is deployed to the Kubernetes cluster.

## Overview

When Whanos detects a `whanos.yml` file at the root of your repository with a `deployment` section, it will:
1. Build and containerize your application
2. Deploy it to the Kubernetes cluster with the specified configuration
3. Create necessary Kubernetes resources (Deployment, Service)

**If `whanos.yml` is missing or has no `deployment` section**, Whanos will only containerize your application without deploying it to Kubernetes.

## File Location

```
your-repo/
├── app/                # Your application code
├── whanos.yml         # ← Must be at repository root
├── Dockerfile         # Optional custom Dockerfile
└── ...
```

## Configuration Structure

```yaml
deployment:              # Root property (required for K8s deployment)
  replicas: 2           # Number of pod replicas (optional)
  resources:            # Resource limits and requests (optional)
    limits:
      memory: "256Mi"
      cpu: "500m"
    requests:
      memory: "128Mi"
      cpu: "250m"
  ports:                # List of ports to expose (optional)
    - 5000
    - 8080
```

## Configuration Options

### `deployment.replicas`

**Type:** Integer
**Default:** 1
**Description:** Number of identical pod instances to run

**Example:**
```yaml
deployment:
  replicas: 3  # Run 3 instances of the application
```

**Use cases:**
- **1 replica:** Development, single-instance apps
- **2+ replicas:** High availability, load distribution
- **3+ replicas:** Production apps requiring redundancy

**Notes:**
- Kubernetes ensures the specified number of replicas are always running
- If a pod crashes, Kubernetes automatically creates a replacement
- Replicas are distributed across available cluster nodes

**Checking replicas:**
```bash
kubectl get deployment <app-name> -n whanos
# Shows READY column: e.g., 3/3 means 3 out of 3 replicas are ready
```

### `deployment.resources`

**Type:** Object
**Default:** No limits (uses cluster defaults)
**Description:** CPU and memory resource constraints

**Syntax follows [Kubernetes resource specifications](https://kubernetes.io/docs/concepts/configuration/manage-resources-containers/)**

#### Resource Fields

**`resources.limits`** - Maximum resources a container can use
- Container will be killed if it exceeds memory limit
- Container will be throttled if it exceeds CPU limit

**`resources.requests`** - Guaranteed minimum resources
- Kubernetes uses this for pod scheduling
- Node must have at least this much resources available

#### Memory Units

```yaml
resources:
  limits:
    memory: "256Mi"    # 256 mebibytes (268,435,456 bytes)
    memory: "1Gi"      # 1 gibibyte
    memory: "512M"     # 512 megabytes (500,000,000 bytes)
```

**Common values:**
- `128Mi` - Small apps, simple services
- `256Mi` - Medium apps, web servers
- `512Mi` - Large apps, data processing
- `1Gi` - Heavy apps, in-memory databases

#### CPU Units

```yaml
resources:
  limits:
    cpu: "500m"    # 500 millicores = 0.5 CPU
    cpu: "1"       # 1 full CPU core
    cpu: "2000m"   # 2000 millicores = 2 CPUs
```

**Common values:**
- `100m` - Very light apps (0.1 CPU)
- `250m` - Small apps (0.25 CPU)
- `500m` - Medium apps (0.5 CPU)
- `1` or `1000m` - CPU-intensive apps (1 full CPU)

#### Complete Example

```yaml
deployment:
  resources:
    limits:           # Maximum allowed
      memory: "512Mi"
      cpu: "1000m"
    requests:         # Guaranteed minimum
      memory: "256Mi"
      cpu: "500m"
```

**Best Practice:** Set requests lower than limits to allow bursting

#### Resource Quota Checking

```bash
# Check pod resource usage
kubectl top pod -n whanos

# Check node resources
kubectl top nodes

# Describe pod to see resource configuration
kubectl describe pod <pod-name> -n whanos
```

### `deployment.ports`

**Type:** Array of integers
**Default:** `[]` (no ports exposed)
**Description:** List of container ports that need to be accessible

**Example:**
```yaml
deployment:
  ports:
    - 5000
    - 8080
```

**What happens when ports are defined:**

1. **Container Ports:** Specified ports are added to pod specification
   ```yaml
   ports:
     - containerPort: 5000
     - containerPort: 8080
   ```

2. **Kubernetes Service:** A service is automatically created
   ```yaml
   apiVersion: v1
   kind: Service
   metadata:
     name: my-app
   spec:
     type: ClusterIP  # Default in Whanos
     ports:
       - port: 5000
         targetPort: 5000
       - port: 8080
         targetPort: 8080
   ```

3. **Service Discovery:** Other pods can reach your app via DNS
   - Within namespace: `http://my-app:5000`
   - From other namespace: `http://my-app.whanos.svc.cluster.local:5000`

**Important Notes:**

- Ports must match what your application listens on
- By default, services use `ClusterIP` type (internal only)
- **External access requires additional configuration** (see [Services and External Access](./services.md))

**Common Port Numbers:**
- `80` - HTTP (web servers)
- `443` - HTTPS (secure web)
- `3000` - Node.js development servers
- `5000` - Flask, Python apps
- `8080` - Alternative HTTP, Java apps
- `8000` - Django development server

## Complete Examples

### Example 1: Simple Web Application

**Python Flask app listening on port 5000:**

```yaml
deployment:
  replicas: 2
  ports:
    - 5000
```

**Result:**
- 2 instances of your app running
- Service exposes port 5000 internally
- No resource limits (uses cluster defaults)

### Example 2: Production Application

**Node.js app with resource management:**

```yaml
deployment:
  replicas: 3
  resources:
    limits:
      memory: "512Mi"
      cpu: "1"
    requests:
      memory: "256Mi"
      cpu: "500m"
  ports:
    - 3000
```

**Result:**
- 3 high-availability replicas
- Each pod guaranteed 256Mi RAM and 0.5 CPU
- Can burst up to 512Mi RAM and 1 CPU
- Port 3000 exposed internally

### Example 3: Microservice with Multiple Ports

**Service exposing HTTP and gRPC:**

```yaml
deployment:
  replicas: 2
  resources:
    limits:
      memory: "256Mi"
      cpu: "500m"
    requests:
      memory: "128Mi"
      cpu: "250m"
  ports:
    - 8080  # HTTP API
    - 9090  # gRPC
```

**Result:**
- 2 replicas for redundancy
- Efficient resource allocation
- Both HTTP and gRPC accessible within cluster

### Example 4: High-Memory Application

**Data processing service:**

```yaml
deployment:
  replicas: 1
  resources:
    limits:
      memory: "2Gi"
      cpu: "2"
    requests:
      memory: "1Gi"
      cpu: "1"
  ports:
    - 8000
```

**Result:**
- Single instance (not parallelizable)
- High memory allocation for data processing
- 2 CPU cores for compute-intensive tasks

### Example 5: Minimal Configuration

**Simple deployment with defaults:**

```yaml
deployment:
  ports:
    - 5000
```

**Result:**
- 1 replica (default)
- No resource limits
- Port 5000 exposed

### Example 6: No Ports (Background Service)

**Worker/cron job that doesn't serve HTTP:**

```yaml
deployment:
  replicas: 1
  resources:
    limits:
      memory: "128Mi"
      cpu: "200m"
```

**Result:**
- Pod runs but no Service is created
- Useful for background workers, cron jobs
- Still benefits from Kubernetes management (restarts, logging)

## No Deployment Section

**If `whanos.yml` is missing or has no deployment section:**

```yaml
# This file has no effect - app won't be deployed to K8s
```

**Or no file at all:**
```
your-repo/
├── app/
└── (no whanos.yml)
```

**Result:**
- Application is containerized and pushed to registry
- **NOT** deployed to Kubernetes
- Image is available for manual deployment

## Validation

### Valid Configuration

```yaml
deployment:
  replicas: 2
  resources:
    limits:
      memory: "256Mi"
      cpu: "500m"
  ports:
    - 5000
```

✅ All fields properly formatted
✅ Memory and CPU use correct units
✅ Ports are integers
✅ Replicas is a positive integer

### Invalid Configurations

❌ **Wrong YAML syntax:**
```yaml
deployment:
replicas: 2  # Wrong indentation
```

❌ **Invalid resource units:**
```yaml
deployment:
  resources:
    limits:
      memory: "256MB"  # Should be "256Mi" or "256M"
      cpu: "half"      # Should be "500m" or "0.5"
```

❌ **Invalid port format:**
```yaml
deployment:
  ports:
    - "5000"  # Should be integer, not string
    - http    # Must be a number
```

❌ **Negative values:**
```yaml
deployment:
  replicas: -1  # Must be positive
```

## Testing Your Configuration

### 1. Validate YAML Syntax

```bash
# Check if whanos.yml is valid YAML
python3 -c "import yaml; yaml.safe_load(open('whanos.yml'))"
```

### 2. Deploy and Check

```bash
# After deployment via Jenkins, check the deployment
kubectl get deployment <app-name> -n whanos

# Check pods
kubectl get pods -n whanos -l app=<app-name>

# Check service
kubectl get service <app-name> -n whanos

# Get full deployment details
kubectl describe deployment <app-name> -n whanos
```

### 3. Verify Resources

```bash
# Check actual resource usage
kubectl top pod -n whanos -l app=<app-name>

# Compare with configured limits
kubectl get pod <pod-name> -n whanos -o jsonpath='{.spec.containers[0].resources}'
```

### 4. Test Port Connectivity

```bash
# Port-forward to test application
kubectl port-forward -n whanos service/<app-name> 8080:5000

# Test with curl
curl http://localhost:8080
```

## Common Patterns

### Development Environment

```yaml
deployment:
  replicas: 1
  resources:
    limits:
      memory: "256Mi"
      cpu: "500m"
  ports:
    - 5000
```

**Characteristics:**
- Single replica (fast restarts)
- Moderate resources
- Simple setup

### Staging Environment

```yaml
deployment:
  replicas: 2
  resources:
    limits:
      memory: "512Mi"
      cpu: "1"
    requests:
      memory: "256Mi"
      cpu: "500m"
  ports:
    - 8080
```

**Characteristics:**
- 2 replicas (tests HA)
- Production-like resources
- Simulates prod environment

### Production Environment

```yaml
deployment:
  replicas: 3
  resources:
    limits:
      memory: "1Gi"
      cpu: "2"
    requests:
      memory: "512Mi"
      cpu: "1"
  ports:
    - 80
    - 443
```

**Characteristics:**
- 3+ replicas (high availability)
- Generous resources with headroom
- Multiple ports (HTTP/HTTPS)

## Troubleshooting

### Issue: Pods not starting

**Check events:**
```bash
kubectl get events -n whanos --sort-by='.lastTimestamp'
kubectl describe pod <pod-name> -n whanos
```

**Common causes:**
- **Insufficient resources:** Node doesn't have enough CPU/memory
- **Invalid port:** Application not listening on specified port
- **Image pull error:** Container image not accessible

**Solution:**
```yaml
# Reduce resource requests
deployment:
  resources:
    requests:
      memory: "128Mi"  # Lower than before
      cpu: "250m"
```

### Issue: Pods keep restarting

**Check logs:**
```bash
kubectl logs <pod-name> -n whanos
kubectl logs <pod-name> -n whanos --previous  # Previous crashed instance
```

**Common causes:**
- **OOMKilled:** Exceeded memory limit
- **Application crash:** Code error
- **Port conflict:** Wrong port configuration

**Solution for OOMKilled:**
```yaml
deployment:
  resources:
    limits:
      memory: "512Mi"  # Increase from 256Mi
```

### Issue: Cannot access application

**Check service:**
```bash
kubectl get service <app-name> -n whanos
kubectl get endpoints <app-name> -n whanos
```

**If endpoints are empty:**
- Pods aren't ready
- Port mismatch between app and whanos.yml

**Solution:**
```yaml
# Ensure port matches what app listens on
deployment:
  ports:
    - 5000  # Must match app.listen(5000) or similar
```

### Issue: Deployment not created

**Check Jenkins logs:**
```bash
# In Jenkins UI, check the build console output
```

**Common causes:**
- **No deployment section in whanos.yml**
- **YAML syntax error**
- **File not at repository root**

**Solution:**
Ensure whanos.yml is valid and at root:
```bash
# Validate locally
yamllint whanos.yml
```

## Best Practices

### 1. Always Set Resource Limits

**Bad:**
```yaml
deployment:
  replicas: 3
  # No resources defined - can starve other pods
```

**Good:**
```yaml
deployment:
  replicas: 3
  resources:
    limits:
      memory: "256Mi"
      cpu: "500m"
    requests:
      memory: "128Mi"
      cpu: "250m"
```

### 2. Requests &lt; Limits

Allow pods to burst when needed:

```yaml
resources:
  limits:
    memory: "512Mi"  # Maximum
    cpu: "1"
  requests:
    memory: "256Mi"  # Guaranteed (50% of limit)
    cpu: "500m"      # Guaranteed (50% of limit)
```

### 3. Use Multiple Replicas for HA

**Single point of failure:**
```yaml
deployment:
  replicas: 1  # If this pod dies, app is down
```

**High availability:**
```yaml
deployment:
  replicas: 3  # Even if 1-2 pods die, app stays up
```

### 4. Match Ports to Application

```yaml
# If your app does:
# app.listen(5000)
# Then whanos.yml should have:
deployment:
  ports:
    - 5000  # ← Must match
```

### 5. Start Conservative, Scale Up

```yaml
# Start with minimal resources
deployment:
  resources:
    limits:
      memory: "128Mi"
      cpu: "250m"

# Monitor usage with: kubectl top pod
# Increase if needed
```

### 6. Version Control Your whanos.yml

Commit whanos.yml to git alongside your code:
```bash
git add whanos.yml
git commit -m "Configure K8s deployment: 2 replicas, 256Mi limit"
```

## Advanced Topics

### Horizontal Pod Autoscaling (Not in Basic Whanos)

Future enhancement could add:
```yaml
deployment:
  replicas: 2
  autoscaling:
    minReplicas: 2
    maxReplicas: 10
    targetCPUUtilizationPercentage: 80
```

### Health Checks (Not in Basic Whanos)

Future enhancement could add:
```yaml
deployment:
  healthCheck:
    livenessProbe:
      httpGet:
        path: /health
        port: 5000
    readinessProbe:
      httpGet:
        path: /ready
        port: 5000
```

## Reference

### Complete whanos.yml Schema

```yaml
# All possible options
deployment:                    # Required for K8s deployment

  replicas: 2                 # Optional, default: 1
                              # Type: Integer > 0

  resources:                  # Optional, default: no limits
    limits:                   # Optional
      memory: "256Mi"         # Optional, K8s memory units
      cpu: "500m"             # Optional, K8s CPU units (millicores)
    requests:                 # Optional
      memory: "128Mi"         # Optional, K8s memory units
      cpu: "250m"             # Optional, K8s CPU units

  ports:                      # Optional, default: []
    - 5000                    # Integer, container port number
    - 8080                    # Can specify multiple ports
```

### Resource Units Reference

**Memory:**
- `Ki` - Kibibyte (1024 bytes)
- `Mi` - Mebibyte (1024² bytes)
- `Gi` - Gibibyte (1024³ bytes)
- `K` - Kilobyte (1000 bytes)
- `M` - Megabyte (1000² bytes)
- `G` - Gigabyte (1000³ bytes)

**CPU:**
- `1` - 1 full CPU core
- `500m` - 500 millicores = 0.5 CPU
- `0.5` - Same as `500m`
- `2000m` - 2 full CPUs

## Related Documentation

- [Services and External Access](./services.md) - How to access deployed applications
- [Kubernetes Deployment](../jenkins/k8s-deployment.md) - How Jenkins deploys to K8s
- [Compatible Repository Structure](../user-guide/compatible-repo.md) - Repository requirements

## Summary

**whanos.yml is optional:**
- Without it (or without `deployment` section): Only builds container image
- With `deployment` section: Builds image AND deploys to Kubernetes

**Key takeaways:**
1. File must be at repository root
2. `deployment` section triggers Kubernetes deployment
3. `replicas` controls number of pod instances (default: 1)
4. `resources` sets memory/CPU limits (default: no limits)
5. `ports` exposes container ports (default: no ports)
6. Services are created as ClusterIP (see [Services docs](./services.md) for external access)

**Minimal working example:**
```yaml
deployment:
  ports:
    - 5000
```

**Production example:**
```yaml
deployment:
  replicas: 3
  resources:
    limits:
      memory: "512Mi"
      cpu: "1"
    requests:
      memory: "256Mi"
      cpu: "500m"
  ports:
    - 8080
```
