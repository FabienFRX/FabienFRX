# Build stage for Flutter Android APK
FROM ubuntu:22.04

# Prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies
RUN apt-get update && apt-get install -y \
    curl \
    git \
    unzip \
    xz-utils \
    zip \
    libglu1-mesa \
    openjdk-17-jdk \
    wget \
    && rm -rf /var/lib/apt/lists/*

# Set JAVA_HOME
ENV JAVA_HOME=/usr/lib/jvm/java-17-openjdk-amd64
ENV PATH=$PATH:$JAVA_HOME/bin

# Install Android SDK
ENV ANDROID_SDK_ROOT=/opt/android-sdk
ENV PATH=$PATH:$ANDROID_SDK_ROOT/cmdline-tools/latest/bin:$ANDROID_SDK_ROOT/platform-tools

RUN mkdir -p $ANDROID_SDK_ROOT/cmdline-tools && \
    cd $ANDROID_SDK_ROOT/cmdline-tools && \
    wget -q https://dl.google.com/android/repository/commandlinetools-linux-9477386_latest.zip && \
    unzip commandlinetools-linux-9477386_latest.zip && \
    rm commandlinetools-linux-9477386_latest.zip && \
    mv cmdline-tools latest

# Accept Android licenses and install required packages
RUN yes | sdkmanager --licenses && \
    sdkmanager "platform-tools" "platforms;android-33" "build-tools;33.0.0"

# Install Flutter
ENV FLUTTER_HOME=/opt/flutter
ENV PATH=$PATH:$FLUTTER_HOME/bin

# Create a non-root user
RUN useradd -ms /bin/bash developer

# Create necessary directories and set permissions
RUN mkdir -p /apk && \
    mkdir -p /opt/android-sdk && \
    mkdir -p /opt/flutter && \
    chown -R developer:developer /opt/android-sdk && \
    chown -R developer:developer /opt/flutter && \
    chown -R developer:developer /apk

# Switch to non-root user for installation
USER developer

RUN git clone https://github.com/flutter/flutter.git -b stable $FLUTTER_HOME && \
    flutter config --no-analytics && \
    flutter precache && \
    flutter doctor

# Set working directory
WORKDIR /app

# Switch back to root to copy files (will be owned by root by default if not specified)
USER root
COPY . .
# Change ownership of the app directory to developer
RUN chown -R developer:developer /app

# Switch back to developer for building
USER developer

# Get dependencies
RUN flutter pub get

# Build APK
RUN flutter build apk --release

# Create output directory and copy APK
RUN mkdir -p /apk && \
    cp build/app/outputs/flutter-apk/app-release.apk /apk/client.apk

# Keep container running
CMD ["tail", "-f", "/dev/null"]
