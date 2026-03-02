# Services and External Access

This guide explains how applications deployed in the Whanos Kubernetes cluster can be accessed from outside the cluster.

## Overview

When you deploy an application with ports defined in `whanos.yml`, Whanos creates:
1. **Kubernetes Deployment** - Runs your application pods
2. **Kubernetes Service** - Exposes your application within the cluster

By default, services are created as `ClusterIP` type, which means they're only accessible within the Kubernetes cluster. To access applications from outside, you have several options.

## Current Whanos Configuration

The Whanos k3d cluster is created with the following port mapping (from `scripts/deploy.sh:104`):

```bash
k3d cluster create whanos-cluster \
    --agents 2 \
    --port "8081:80@loadbalancer"
```

This configuration:
- Maps **host port 8081** → **cluster LoadBalancer port 80**
- Enables external access to services exposed through the LoadBalancer

## Service Types in Kubernetes

### ClusterIP (Default)

**Current implementation in Jenkinsfile.template:197**

```yaml
type: ClusterIP
```

**Characteristics:**
- Service is only accessible within the cluster
- Not accessible from outside (localhost or internet)
- Most secure option
- Requires additional configuration for external access

**Use case:** Internal microservices, databases, cache services

### LoadBalancer

**Recommended for Whanos with k3d**

```yaml
type: LoadBalancer
```

**Characteristics:**
- Exposes service externally
- With k3d's port mapping `8081:80`, services are accessible on `localhost:8081`
- Multiple services will share the same external IP/port
- Requires Ingress controller or unique ports for multiple apps

**How it works with k3d:**
1. Service requests LoadBalancer IP
2. k3d's built-in LoadBalancer provides an IP
3. Traffic to host port 8081 → LoadBalancer → Service → Pod

### NodePort

**Alternative option**

```yaml
type: NodePort
ports:
  - port: 5000
    targetPort: 5000
    nodePort: 30500  # Must be in range 30000-32767
```

**Characteristics:**
- Exposes service on each node's IP at a static port
- Port range: 30000-32767
- Accessible via `<NodeIP>:<NodePort>`
- Good for development, not recommended for production

### ExternalName

Maps service to a DNS name. Not applicable for Whanos use case.

## Methods to Access Applications Externally

### Method 1: Using kubectl port-forward (Development)

**Quickest method for testing**

```bash
# Forward local port 8080 to application port 5000
kubectl port-forward -n whanos service/<app-name> 8080:5000

# Example: Forward port 8080 to python-app on port 5000
kubectl port-forward -n whanos service/python-app 8080:5000
```

**Access:** `http://localhost:8080`

**Advantages:**
- No configuration changes needed
- Works with ClusterIP services
- Great for debugging

**Disadvantages:**
- Only works while command is running
- Single connection
- Not suitable for production

### Method 2: Change Service Type to LoadBalancer

**Modify Jenkinsfile.template** to use LoadBalancer instead of ClusterIP:

**Current code (line 197):**
```groovy
type: ClusterIP
```

**Modified code:**
```groovy
type: LoadBalancer
```

**Complete service definition:**
```yaml
apiVersion: v1
kind: Service
metadata:
  name: my-app
  namespace: whanos
spec:
  selector:
    app: my-app
  ports:
    - protocol: TCP
      port: 80        # Service port
      targetPort: 5000  # Container port
  type: LoadBalancer
```

**Access:**
- With k3d port mapping `8081:80`: `http://localhost:8081`
- **Important:** Only ONE service can use port 80 at a time with this configuration

**Advantages:**
- Simple configuration
- Works well with k3d
- Automatic load balancing

**Disadvantages:**
- Only one app can use port 80
- Need Ingress for multiple apps

### Method 3: Using Ingress Controller (Recommended for Production)

**Setup Nginx Ingress Controller:**

```bash
# Install Nginx Ingress
kubectl apply -f https://raw.githubusercontent.com/kubernetes/ingress-nginx/main/deploy/static/provider/cloud/deploy.yaml

# Wait for controller to be ready
kubectl wait --namespace ingress-nginx \
  --for=condition=ready pod \
  --selector=app.kubernetes.io/component=controller \
  --timeout=120s
```

**Create Ingress resource** (add to Jenkinsfile deployment logic):

```yaml
apiVersion: networking.k8s.io/v1
kind: Ingress
metadata:
  name: my-app-ingress
  namespace: whanos
  annotations:
    nginx.ingress.kubernetes.io/rewrite-target: /
spec:
  ingressClassName: nginx
  rules:
    - host: my-app.local
      http:
        paths:
          - path: /
            pathType: Prefix
            backend:
              service:
                name: my-app
                port:
                  number: 5000
```

**Add to /etc/hosts:**
```bash
echo "127.0.0.1 my-app.local" | sudo tee -a /etc/hosts
```

**Access:** `http://my-app.local:8081`

**Advantages:**
- Multiple applications on same port
- Path-based routing
- Host-based routing
- SSL/TLS termination
- Production-ready

**Disadvantages:**
- More complex setup
- Requires Ingress controller installation

### Method 4: Using NodePort

**Modify Jenkinsfile.template** to use NodePort:

```groovy
// Change type to NodePort and add nodePort specification
if (ports) {
    servicePorts = ports.collect { port ->
        def nodePort = 30000 + (port.toInteger() % 2000)  // Generate NodePort in range
        """
  - protocol: TCP
    port: ${port}
    targetPort: ${port}
    nodePort: ${nodePort}"""
    }.join('')

    k8sManifest += """
---
apiVersion: v1
kind: Service
metadata:
  name: ${env.APP_NAME}
  namespace: whanos
spec:
  selector:
    app: ${env.APP_NAME}
  ports:${servicePorts}
  type: NodePort
"""
}
```

**Find NodePort:**
```bash
kubectl get service <app-name> -n whanos
```

**Access:**
```bash
# Get node IP (usually localhost for k3d)
kubectl get nodes -o wide

# Access via NodePort
# http://localhost:<NodePort>
# Example: http://localhost:30500
```

**Advantages:**
- Simple to configure
- No additional components needed
- Good for development

**Disadvantages:**
- Limited port range (30000-32767)
- Need to remember/document NodePorts
- Ports may conflict with host services

## Implementation Options for Whanos

### Option A: Simple LoadBalancer (Quick Solution)

**Pros:**
- Minimal code changes
- Works immediately with k3d
- Good for single-app deployments

**Cons:**
- Only one app accessible at port 8081 at a time
- Not suitable for multiple simultaneous deployments

**Implementation:**
```groovy
// In Jenkinsfile.template line 197, change:
type: ClusterIP
// To:
type: LoadBalancer
```

### Option B: Ingress Controller (Production-Ready)

**Pros:**
- Multiple apps accessible simultaneously
- Professional solution
- Scalable
- Supports SSL/TLS

**Cons:**
- Requires Ingress controller setup
- More complex configuration
- Need to manage DNS/hosts entries

**Implementation:**
1. Install Ingress controller (one-time setup)
2. Modify Jenkinsfile to create Ingress resources
3. Configure hostname-based routing

### Option C: NodePort (Development Friendly)

**Pros:**
- Easy to understand
- No additional components
- Each app gets unique port

**Cons:**
- Port management complexity
- Non-standard ports
- Not production-recommended

**Implementation:**
```groovy
// In Jenkinsfile.template, modify service creation to use NodePort
type: NodePort
```

### Option D: Hybrid Approach (Recommended)

**Use ClusterIP + kubectl port-forward for development:**
- Keep services as ClusterIP
- Document port-forward commands
- Simple and secure

**Use Ingress for production deployment:**
- Install Ingress controller for production
- Configure Ingress rules
- Professional setup

## Examples

### Example 1: Port-Forward to Python App

**App deployed with whanos.yml:**
```yaml
deployment:
  replicas: 2
  ports:
    - 5000
```

**Access from localhost:**
```bash
# Find the service
kubectl get service -n whanos

# Port-forward
kubectl port-forward -n whanos service/python-app 8080:5000

# Access
curl http://localhost:8080
```

### Example 2: LoadBalancer Service

**Modified Jenkinsfile creates:**
```yaml
apiVersion: v1
kind: Service
metadata:
  name: python-app
  namespace: whanos
spec:
  selector:
    app: python-app
  ports:
    - protocol: TCP
      port: 80
      targetPort: 5000
  type: LoadBalancer
```

**Access:**
```bash
# Wait for LoadBalancer to get external IP
kubectl get service python-app -n whanos
# NAME         TYPE           CLUSTER-IP     EXTERNAL-IP   PORT(S)        AGE
# python-app   LoadBalancer   10.43.50.123   <pending>     80:31234/TCP   10s

# With k3d, access via mapped port
curl http://localhost:8081

# Or use the NodePort shown (31234 in this example)
curl http://localhost:31234
```

### Example 3: Ingress with Multiple Apps

**Install Ingress:**
```bash
kubectl apply -f https://raw.githubusercontent.com/kubernetes/ingress-nginx/main/deploy/static/provider/cloud/deploy.yaml
```

**Create Ingress for app1:**
```yaml
apiVersion: networking.k8s.io/v1
kind: Ingress
metadata:
  name: app1-ingress
  namespace: whanos
spec:
  ingressClassName: nginx
  rules:
    - host: app1.whanos.local
      http:
        paths:
          - path: /
            pathType: Prefix
            backend:
              service:
                name: app1
                port:
                  number: 5000
```

**Create Ingress for app2:**
```yaml
apiVersion: networking.k8s.io/v1
kind: Ingress
metadata:
  name: app2-ingress
  namespace: whanos
spec:
  ingressClassName: nginx
  rules:
    - host: app2.whanos.local
      http:
        paths:
          - path: /
            pathType: Prefix
            backend:
              service:
                name: app2
                port:
                  number: 8080
```

**Configure hosts:**
```bash
sudo tee -a /etc/hosts <<EOF
127.0.0.1 app1.whanos.local
127.0.0.1 app2.whanos.local
EOF
```

**Access:**
```bash
curl http://app1.whanos.local:8081
curl http://app2.whanos.local:8081
```

## Checking Service Status

### View Services

```bash
# List all services in whanos namespace
kubectl get services -n whanos

# Get detailed info about a specific service
kubectl describe service <app-name> -n whanos

# Get service with more details
kubectl get service <app-name> -n whanos -o wide
```

### View Endpoints

```bash
# Check which pods are behind a service
kubectl get endpoints <app-name> -n whanos

# Detailed endpoint information
kubectl describe endpoints <app-name> -n whanos
```

### Test Service Connectivity

```bash
# From within the cluster (using a debug pod)
kubectl run -n whanos debug-pod --rm -it --image=curlimages/curl -- sh
# Then inside the pod:
curl http://<app-name>:<port>

# From outside using port-forward
kubectl port-forward -n whanos service/<app-name> 8080:<port>
curl http://localhost:8080
```

## Troubleshooting External Access

### Issue: Cannot access service from localhost

**Check service type:**
```bash
kubectl get service <app-name> -n whanos -o jsonpath='{.spec.type}'
```

**If ClusterIP:**
- Use `kubectl port-forward` for testing
- Change to LoadBalancer or NodePort for permanent access
- Or set up Ingress

### Issue: LoadBalancer external IP stuck on &lt;pending&gt;

**With k3d, this is normal:**
- k3d provides built-in LoadBalancer support
- Access via the port mapping: `localhost:8081`
- Or use the automatically assigned NodePort

**Check:**
```bash
kubectl get service <app-name> -n whanos
# Note the NodePort in PORT(S) column (e.g., 80:31234/TCP)
# Access via: http://localhost:31234
```

### Issue: Port 8081 connection refused

**Check k3d cluster status:**
```bash
k3d cluster list
docker ps | grep k3d
```

**Verify port mapping:**
```bash
docker port k3d-whanos-cluster-serverlb
# Should show: 80/tcp -> 0.0.0.0:8081
```

**Check if any service is using LoadBalancer:**
```bash
kubectl get services -n whanos -o wide
```

### Issue: Multiple apps, only one accessible

**Problem:** Multiple LoadBalancer services competing for port 80

**Solutions:**
1. Use Ingress controller with host-based routing
2. Use different service ports:
   ```yaml
   ports:
     - port: 8080  # app1
     - port: 8090  # app2
   ```
3. Use NodePort with unique ports

### Issue: Ingress not working

**Check Ingress controller:**
```bash
kubectl get pods -n ingress-nginx
kubectl logs -n ingress-nginx <ingress-controller-pod>
```

**Check Ingress resource:**
```bash
kubectl get ingress -n whanos
kubectl describe ingress <app-name>-ingress -n whanos
```

**Verify Ingress class:**
```bash
kubectl get ingressclass
```

## Recommendations

### For Development (Local Testing)

**Use kubectl port-forward:**
- No infrastructure changes needed
- Quick and simple
- Easy cleanup

```bash
kubectl port-forward -n whanos service/my-app 8080:5000
```

### For Demo/Presentation

**Use LoadBalancer or NodePort:**
- Persistent access
- No manual forwarding needed
- Easy to demonstrate

**Configure once, access always:**
```bash
# After deployment, get NodePort
kubectl get service my-app -n whanos
# Access via http://localhost:<NodePort>
```

### For Production Deployment

**Use Ingress Controller:**
- Professional solution
- Multiple apps support
- SSL/TLS support
- Proper load balancing

**Setup Ingress controller + configure Ingress resources**

## Next Steps

- [whanos.yml Configuration](./whanos-yml.md) - Learn how to configure ports in whanos.yml
- [Kubernetes Deployment](../jenkins/k8s-deployment.md) - Understand the deployment process
- [Troubleshooting](../troubleshooting/common-issues.md) - Common issues and solutions

## Summary

**Current Whanos Setup:**
- Services are created as `ClusterIP` (internal only)
- k3d cluster has port mapping `8081:80` ready for LoadBalancer
- Applications are NOT accessible externally by default

**To Make Apps Accessible:**

| Method | Difficulty | Production Ready | Multiple Apps |
|--------|-----------|------------------|---------------|
| port-forward | Easy | No | Yes (manual) |
| LoadBalancer | Easy | Yes (1 app) | No (port conflicts) |
| NodePort | Medium | No | Yes (unique ports) |
| Ingress | Hard | Yes | Yes (host routing) |

**Quick Fix:**
Change `type: ClusterIP` to `type: LoadBalancer` in Jenkinsfile.template:197

**Best Practice:**
Implement Ingress controller for proper multi-app support
