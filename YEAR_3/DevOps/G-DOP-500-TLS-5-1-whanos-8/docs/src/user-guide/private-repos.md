# Working with Private Repositories

Whanos fully supports private Git repositories. This guide explains how to configure Git credentials in Jenkins and use them with your private repositories.

## Overview

Private repositories require authentication to clone and access code. Whanos integrates with Jenkins' credential management system to securely store and use Git credentials.

**Supported authentication methods:**
- Username and Password
- Personal Access Tokens (recommended)
- SSH Keys

## Why Use Personal Access Tokens?

Personal access tokens (PATs) are more secure than passwords because:
- They can be scoped with specific permissions
- They can be revoked individually without changing your password
- They expire automatically (on most platforms)
- Many platforms (GitHub, GitLab) now require PATs instead of passwords

## Creating Personal Access Tokens

### GitHub

1. Go to GitHub Settings → Developer settings → Personal access tokens → Tokens (classic)
2. Click "Generate new token (classic)"
3. Give it a descriptive name (e.g., "Whanos CI/CD")
4. Set expiration (90 days recommended, or custom)
5. Select scopes:
   - `repo` (full control of private repositories)
   - `read:org` (if using organization repositories)
6. Click "Generate token"
7. **Copy the token immediately** (you won't see it again)

**Token format:** `ghp_xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx`

### GitLab

1. Go to GitLab Preferences → Access Tokens
2. Enter token name (e.g., "Whanos")
3. Set expiration date (optional but recommended)
4. Select scopes:
   - `read_repository`
   - `write_repository`
5. Click "Create personal access token"
6. **Copy the token immediately**

**Token format:** `glpat-xxxxxxxxxxxxxxxxxxxx`

### Bitbucket

1. Go to Bitbucket Settings → Personal access tokens
2. Click "Create token"
3. Enter label (e.g., "Whanos CI/CD")
4. Set permissions:
   - Repositories: Read, Write
5. Click "Create"
6. **Copy the token immediately**

## Adding Credentials to Jenkins

### Method 1: Jenkins Web UI (Recommended)

1. **Access Jenkins:**
   ```bash
   # Open Jenkins in your browser
   http://localhost:8080
   ```

2. **Navigate to Credentials:**
   - Click "Manage Jenkins" (left sidebar)
   - Click "Credentials"
   - Click "(global)" under "Stores scoped to Jenkins"
   - Click "Add Credentials" (left sidebar)

3. **Configure Credential:**

   **For Personal Access Token (recommended):**
   - Kind: `Username with password`
   - Scope: `Global (Jenkins, nodes, items, all child items, etc)`
   - Username: Your Git username (e.g., `johndoe`)
   - Password: Your personal access token (paste the token you copied)
   - ID: A unique identifier (e.g., `github-whanos`, `gitlab-private-repo`)
   - Description: Human-readable description (e.g., "GitHub PAT for private repos")

   **For SSH Key:**
   - Kind: `SSH Username with private key`
   - Scope: `Global`
   - ID: Unique identifier (e.g., `github-ssh`)
   - Username: `git`
   - Private Key: Choose "Enter directly" and paste your private SSH key
   - Passphrase: If your key is encrypted, enter passphrase
   - Description: Description of the credential

4. **Click "Create"**

### Method 2: Jenkins Configuration as Code (JCasC)

You can also pre-configure credentials in `jenkins/config/jenkins.yaml`:

```yaml
credentials:
  system:
    domainCredentials:
      - credentials:
          - usernamePassword:
              scope: GLOBAL
              id: "github-whanos"
              username: "your-username"
              password: "{AQAAABAAAAAw...}"  # Encrypted secret
              description: "GitHub Personal Access Token"

          - basicSSHUserPrivateKey:
              scope: GLOBAL
              id: "github-ssh"
              username: "git"
              privateKeySource:
                directEntry:
                  privateKey: "{AQAAABAAAAAw...}"  # Encrypted key
              description: "GitHub SSH Key"
```

**Note:** Secrets should be encrypted using Jenkins secret encryption. For simplicity, use the Web UI method.

## Using Credentials in link-project Job

When creating a new project via the `link-project` job, you specify the credential ID:

### Via Jenkins UI

1. **Go to the link-project job:**
   ```
   http://localhost:8080/job/Whanos/job/link-project/
   ```

2. **Click "Build with Parameters"**

3. **Fill in the parameters:**
   - **DISPLAY_NAME:** Project name (e.g., `my-private-app`)
   - **GIT_URL:** Private repository URL
     - HTTPS: `https://github.com/username/private-repo.git`
     - SSH: `git@github.com:username/private-repo.git`
   - **CREDENTIAL_ID:** The ID you created in Jenkins (e.g., `github-whanos`)

4. **Click "Build"**

### Via Jenkins CLI/API

```bash
# Using Jenkins CLI
java -jar jenkins-cli.jar -s http://localhost:8080/ build link-project \
  -p DISPLAY_NAME="my-private-app" \
  -p GIT_URL="https://github.com/username/private-repo.git" \
  -p CREDENTIAL_ID="github-whanos"

# Using curl
curl -X POST 'http://localhost:8080/job/Whanos/job/link-project/buildWithParameters' \
  --user admin:admin \
  --data-urlencode DISPLAY_NAME="my-private-app" \
  --data-urlencode GIT_URL="https://github.com/username/private-repo.git" \
  --data-urlencode CREDENTIAL_ID="github-whanos"
```

### What Happens Next

1. Jenkins creates a new job under `Whanos/Projects/`
2. The job is configured with your credential ID
3. On the first build, Jenkins clones the private repository using the credentials
4. The pipeline executes normally (detect language, build, test, deploy)

## Repository URL Formats

### HTTPS (Use with Personal Access Tokens)

```
# GitHub
https://github.com/username/repo.git

# GitLab
https://gitlab.com/username/repo.git

# Bitbucket
https://bitbucket.org/username/repo.git
```

**Advantages:**
- Works through corporate firewalls
- Easier to set up
- Token-based authentication

### SSH (Use with SSH Keys)

```
# GitHub
git@github.com:username/repo.git

# GitLab
git@gitlab.com:username/repo.git

# Bitbucket
git@bitbucket.org:username/repo.git
```

**Advantages:**
- More secure (key-based)
- No password prompts
- Common in enterprise environments

**Requirements:**
- SSH key pair generated
- Public key added to Git platform
- Private key added to Jenkins

## SSH Key Setup

If you prefer SSH authentication:

### 1. Generate SSH Key Pair

```bash
# On your local machine or Jenkins server
ssh-keygen -t ed25519 -C "whanos-jenkins" -f ~/.ssh/whanos_key

# This creates:
# - ~/.ssh/whanos_key (private key - add to Jenkins)
# - ~/.ssh/whanos_key.pub (public key - add to Git platform)
```

### 2. Add Public Key to Git Platform

**GitHub:**
1. Go to Settings → SSH and GPG keys → New SSH key
2. Paste content of `~/.ssh/whanos_key.pub`
3. Click "Add SSH key"

**GitLab:**
1. Go to Preferences → SSH Keys
2. Paste content of `~/.ssh/whanos_key.pub`
3. Click "Add key"

**Bitbucket:**
1. Go to Personal settings → SSH keys
2. Click "Add key"
3. Paste content of `~/.ssh/whanos_key.pub`

### 3. Add Private Key to Jenkins

1. Copy the **private key** content:
   ```bash
   cat ~/.ssh/whanos_key
   ```

2. In Jenkins:
   - Manage Jenkins → Credentials → Add Credentials
   - Kind: `SSH Username with private key`
   - ID: `github-ssh`
   - Username: `git`
   - Private Key: Choose "Enter directly", paste the private key
   - Click "Create"

### 4. Use SSH URL in link-project

Use the SSH format URL (e.g., `git@github.com:username/repo.git`) and specify your SSH credential ID.

## Testing Credentials

### Test from Jenkins Script Console

You can verify credentials work before creating projects:

1. Go to Manage Jenkins → Script Console
2. Run this Groovy script:

```groovy
import jenkins.model.Jenkins
import com.cloudbees.plugins.credentials.*
import com.cloudbees.plugins.credentials.common.*
import com.cloudbees.plugins.credentials.domains.*

// List all credentials
def credentials = CredentialsProvider.lookupCredentials(
    StandardCredentials.class,
    Jenkins.instance,
    null,
    null
)

credentials.each { c ->
    println("ID: ${c.id}, Description: ${c.description}")
}
```

This lists all available credentials to ensure yours was created correctly.

### Test with a Manual Git Clone

1. Access the Jenkins container:
   ```bash
   docker exec -it whanos-jenkins bash
   ```

2. Test HTTPS clone with token:
   ```bash
   git clone https://USERNAME:TOKEN@github.com/username/private-repo.git /tmp/test-clone
   ```

3. Test SSH clone:
   ```bash
   # First, add SSH key to ssh-agent
   eval $(ssh-agent)
   ssh-add /path/to/private/key

   # Then clone
   git clone git@github.com:username/private-repo.git /tmp/test-ssh-clone
   ```

## Troubleshooting

### Error: "Authentication failed"

**Cause:** Invalid credentials or expired token

**Solution:**
1. Verify the token/password is correct
2. Check if the token has expired
3. Ensure the token has correct permissions (`repo` scope for GitHub)
4. Regenerate the token if necessary
5. Update the credential in Jenkins

### Error: "Repository not found"

**Causes:**
- Repository URL is incorrect
- Credentials don't have access to the repository
- Repository name changed

**Solution:**
1. Verify the repository URL is correct
2. Check that the Git user has access to the repository
3. Ensure credential ID is specified in link-project parameters

### Error: "Permission denied (publickey)"

**Cause:** SSH key not recognized

**Solution:**
1. Verify public key is added to Git platform
2. Check private key is correctly added to Jenkins
3. Ensure username is `git` (not your Git username)
4. Test SSH connection:
   ```bash
   ssh -T git@github.com
   # Should output: Hi username! You've successfully authenticated...
   ```

### Error: "Host key verification failed"

**Cause:** Jenkins doesn't trust the Git host's SSH key

**Solution:**
1. Add Git host to Jenkins known_hosts:
   ```bash
   docker exec -it whanos-jenkins bash
   ssh-keyscan github.com >> ~/.ssh/known_hosts
   ssh-keyscan gitlab.com >> ~/.ssh/known_hosts
   ssh-keyscan bitbucket.org >> ~/.ssh/known_hosts
   ```

### Credential Not Found in Job

**Symptoms:** Job shows "Credential not found" or uses anonymous clone

**Solution:**
1. Verify credential ID matches exactly (case-sensitive)
2. Ensure credential scope is "Global"
3. Check that the credential exists:
   - Manage Jenkins → Credentials
4. Re-run link-project with correct CREDENTIAL_ID parameter

### Token Permissions Insufficient

**Symptoms:** Clone works but push fails, or certain operations fail

**Solution:**
- **GitHub:** Ensure token has `repo` scope (full control)
- **GitLab:** Ensure token has `write_repository` scope
- **Bitbucket:** Ensure token has "Repositories: Write" permission

## Security Best Practices

### 1. Use Tokens, Not Passwords

Always use personal access tokens instead of your actual password:
- Tokens can be revoked without changing your password
- Tokens can have limited scopes
- Many platforms now require tokens for API access

### 2. Set Token Expiration

Configure tokens to expire after a reasonable time:
- 90 days for active development
- 30 days for testing/experimentation
- Set calendar reminders to rotate tokens before expiration

### 3. Minimum Scope Principle

Only grant the minimum permissions needed:
- For read-only deployments: read access only
- For CI/CD: read + write to specific repos
- Avoid `admin` or `delete` permissions unless absolutely necessary

### 4. Rotate Credentials Regularly

Rotate tokens and SSH keys periodically:
- Every 90 days for production
- Immediately if a credential may have been compromised
- After team member departures

### 5. Use Organization-Level Tokens

For team projects, consider:
- GitHub: Use deploy keys or GitHub Apps instead of personal tokens
- GitLab: Use project access tokens
- Bitbucket: Use workspace access tokens

This ensures credentials aren't tied to a single person.

### 6. Audit Credential Usage

Regularly review:
- Which credentials are configured in Jenkins
- Which jobs use which credentials
- When credentials were last used
- Remove unused credentials

### 7. Encrypt Credentials in JCasC

If using Jenkins Configuration as Code:
```bash
# Encrypt secrets using Jenkins CLI
echo -n 'my-secret-token' | java -jar jenkins-cli.jar -s http://localhost:8080/ \
  groovy = <<< "println(hudson.util.Secret.fromString(System.in.text).getEncryptedValue())"
```

Use the encrypted value in `jenkins.yaml`.

### 8. Network-Level Security

For production deployments:
- Use HTTPS for Jenkins access
- Enable Jenkins authentication and authorization
- Restrict network access to Jenkins (firewall rules)
- Use VPN for Jenkins access if handling sensitive repos

## Multiple Credentials

You can create multiple credentials for different repositories or organizations:

```
github-public-repos     → Your public GitHub projects
github-private-work     → Company private repositories
gitlab-personal         → Personal GitLab projects
bitbucket-team          → Team Bitbucket workspace
```

When running link-project, specify the appropriate credential ID for each project.

## Advanced: Credential Binding in Pipelines

The Whanos Jenkinsfile template already handles credentials, but for custom modifications:

```groovy
// In Jenkinsfile
checkout([
    $class: 'GitSCM',
    branches: [[name: "*/${env.GIT_BRANCH}"]],
    userRemoteConfigs: [[
        url: env.GIT_URL,
        credentialsId: env.CREDENTIAL_ID  // Passed from link-project
    ]]
])
```

This is already configured in the Whanos template, so you don't need to modify it.

## Examples

### Example 1: GitHub Private Repository with Token

1. **Create Token:** GitHub Settings → Developer settings → Personal access tokens
   - Scopes: `repo`
   - Copy token: `ghp_xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx`

2. **Add to Jenkins:**
   - ID: `github-myapp`
   - Username: `johndoe`
   - Password: `ghp_xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx`

3. **Link Project:**
   - DISPLAY_NAME: `my-backend`
   - GIT_URL: `https://github.com/johndoe/my-backend-app.git`
   - CREDENTIAL_ID: `github-myapp`

### Example 2: GitLab Private Repository with SSH

1. **Generate SSH Key:**
   ```bash
   ssh-keygen -t ed25519 -f ~/.ssh/gitlab_whanos
   ```

2. **Add Public Key to GitLab:**
   - Copy: `cat ~/.ssh/gitlab_whanos.pub`
   - GitLab → Preferences → SSH Keys → Add key

3. **Add Private Key to Jenkins:**
   - Kind: SSH Username with private key
   - ID: `gitlab-ssh`
   - Username: `git`
   - Private Key: Content of `~/.ssh/gitlab_whanos`

4. **Link Project:**
   - DISPLAY_NAME: `my-frontend`
   - GIT_URL: `git@gitlab.com:username/my-frontend.git`
   - CREDENTIAL_ID: `gitlab-ssh`

### Example 3: Organization Repository

For company/organization repositories:

1. **Create Organization Token** (preferred over personal tokens)
   - GitHub: Settings → Developer settings → Personal access tokens → Fine-grained tokens
   - Limit to specific organization/repositories
   - Set resource owner to organization

2. **Add to Jenkins:**
   - ID: `company-repos`
   - Username: Service account username
   - Password: Organization token

3. **Link Project:**
   - GIT_URL: `https://github.com/mycompany/backend-service.git`
   - CREDENTIAL_ID: `company-repos`

## Next Steps

- [Creating Compatible Repositories](./compatible-repo.md) - How to structure your app for Whanos
- [Deployment Workflow](./deployment.md) - Understanding the CI/CD pipeline
- [Troubleshooting](../troubleshooting/common-issues.md) - Common issues and solutions

## Summary

To use private repositories with Whanos:

1. ✅ Create a personal access token on your Git platform
2. ✅ Add credentials to Jenkins (Manage Jenkins → Credentials)
3. ✅ Note the credential ID you create
4. ✅ Run link-project job with the credential ID
5. ✅ Your private repository will be automatically deployed

**Remember:** Keep your tokens secure, rotate them regularly, and use the minimum required permissions.
