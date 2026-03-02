 # Whanos — quick overview and setup

 This repository contains a small CI/CD playground named *Whanos* that runs locally.

 Core components (run as local Docker containers):
 - Jenkins (CI server)
 - A local Docker registry
 - A small Kubernetes test cluster (created with `k3d` by the deploy script)

 The infrastructure is declared with Terraform modules under `terraform/modules` and environment configuration under `terraform/environments/dev`.

 ## Prerequisites

 - Docker (required and running)
 - Git
 - Optional: Terraform (if you want to run the Terraform steps manually)

 ## Quick start

 From the repository root, run the one-line deploy script:

 ```bash
 ./scripts/deploy.sh
 ```

 That script will check for prerequisites, create a small `k3d` cluster if needed, and run Terraform to start Jenkins and the local registry.

 ## Run Terraform directly

 If you prefer Terraform directly:

 ```bash
 cd terraform/environments/dev
 terraform init
 terraform apply
 ```

 ## Stop and clean up

 Run the destroy script to remove created resources:

 ```bash
 ./scripts/destroy.sh
 ```

 Or with Terraform only:

 ```bash
 cd terraform/environments/dev
 terraform destroy
 ```

 ## Where to look next

 - Terraform code: `terraform/environments/dev` and `terraform/modules`
 - Scripts: `scripts/deploy.sh` and `scripts/destroy.sh`
 - More docs: `docs/` folder

 ---
 This README keeps things intentionally minimal — use the `docs/` folder for detailed guides.
