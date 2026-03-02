FROM jenkins/jenkins:lts

USER root

RUN apt-get update && \
    apt-get install -y git make gcc && \
    apt-get clean

RUN jenkins-plugin-cli --plugins \
    cloudbees-folder \
    configuration-as-code \
    credentials \
    github \
    instance-identity \
    job-dsl \
    script-security \
    structs \
    role-strategy \
    ws-cleanup

USER jenkins