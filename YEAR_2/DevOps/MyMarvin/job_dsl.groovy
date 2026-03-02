folder('Tools') {
    description('Folder for miscellaneous tools.')
}

job('Tools/SEED') {
    description('This job generates other jobs using DSL.')

    parameters {
        stringParam('GITHUB_NAME', '', 'GitHub repository owner/repo_name (e.g.: "EpitechIT31000/chocolatine")')
        stringParam('DISPLAY_NAME', '', 'Display name for the job')
    }

    steps {
        dsl {
            text("""
                job("\${DISPLAY_NAME}") {
                    properties {
                        githubProjectUrl('https://github.com/\${GITHUB_NAME}')
                    }

                    scm {
                        github("\${GITHUB_NAME}")
                    }

                    wrappers {
                        preBuildCleanup()
                    }

                    triggers {
                        scm('* * * * *')
                    }

                    steps {
                        shell('make fclean')
                        shell('make')
                        shell('make tests_run')
                        shell('make clean')
                    }
                }
            """)
        }
    }
}

job('Tools/clone-repository') {
    description('This job clones a Git repository.')

    parameters {
        stringParam('GIT_REPOSITORY_URL', '', 'Git URL of the repository to clone')
    }

    wrappers {
        preBuildCleanup()
    }

    steps {
        shell('git clone ${GIT_REPOSITORY_URL}')
    }
}