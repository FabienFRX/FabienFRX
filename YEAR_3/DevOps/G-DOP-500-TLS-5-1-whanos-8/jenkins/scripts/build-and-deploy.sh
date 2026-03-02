#!/bin/bash
set -e

detect_language() {
    if [ -f "Makefile" ]; then
        echo "c"
    elif [ -f "app/pom.xml" ]; then
        echo "java"
    elif [ -f "package.json" ]; then
        echo "javascript"
    elif [ -f "requirements.txt" ]; then
        echo "python"
    elif [ -f "app/main.bf" ]; then
        echo "befunge"
    else
        echo "unknown"
    fi
}

LANGUAGE=$(detect_language)
IMAGE_TAG="${ACR_SERVER}/${PROJECT_NAME}:${BUILD_NUMBER}"
IMAGE_LATEST="${ACR_SERVER}/${PROJECT_NAME}:latest"

echo "Detected language: $LANGUAGE"

if [ "$LANGUAGE" == "unknown" ]; then
    echo "Error: Unable to detect project language"
    exit 1
fi

echo $ACR_PASSWORD | docker login $ACR_SERVER -u $ACR_USERNAME --password-stdin

if [ -f "Dockerfile" ]; then
    echo "Using custom Dockerfile"
    docker build -t $IMAGE_TAG .
else
    echo "Using Whanos $LANGUAGE standalone image"
    docker build -f /var/jenkins_home/whanos-images/${LANGUAGE}/Dockerfile.standalone -t $IMAGE_TAG .
fi

docker push $IMAGE_TAG
docker tag $IMAGE_TAG $IMAGE_LATEST
docker push $IMAGE_LATEST

if [ -f "whanos.yml" ]; then
    echo "Deploying to Kubernetes..."
    kubectl apply -f - <<EOF
apiVersion: apps/v1
kind: Deployment
metadata:
  name: ${PROJECT_NAME}
  namespace: whanos
spec:
  replicas: 1
  selector:
    matchLabels:
      app: ${PROJECT_NAME}
  template:
    metadata:
      labels:
        app: ${PROJECT_NAME}
    spec:
      containers:
      - name: app
        image: ${IMAGE_TAG}
EOF
fi
