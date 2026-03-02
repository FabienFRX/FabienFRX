// Populate the sidebar
//
// This is a script, and not included directly in the page, to control the total size of the book.
// The TOC contains an entry for each page, so if each page includes a copy of the TOC,
// the total size of the page becomes O(n**2).
class MDBookSidebarScrollbox extends HTMLElement {
    constructor() {
        super();
    }
    connectedCallback() {
        this.innerHTML = '<ol class="chapter"><li class="chapter-item affix "><a href="introduction.html">Introduction</a></li><li class="chapter-item affix "><li class="part-title">Getting Started</li><li class="chapter-item "><a href="quick-start.html"><strong aria-hidden="true">1.</strong> Quick Start</a></li><li class="chapter-item "><a href="prerequisites.html"><strong aria-hidden="true">2.</strong> Prerequisites</a></li><li class="chapter-item "><a href="installation.html"><strong aria-hidden="true">3.</strong> Installation</a></li><li class="chapter-item affix "><li class="part-title">Architecture</li><li class="chapter-item "><a href="architecture/overview.html"><strong aria-hidden="true">4.</strong> Overview</a></li><li class="chapter-item "><a href="architecture/components.html"><strong aria-hidden="true">5.</strong> Components</a><a class="toggle"><div>❱</div></a></li><li><ol class="section"><li class="chapter-item "><a href="architecture/jenkins.html"><strong aria-hidden="true">5.1.</strong> Jenkins</a></li><li class="chapter-item "><a href="architecture/registry.html"><strong aria-hidden="true">5.2.</strong> Docker Registry</a></li><li class="chapter-item "><a href="architecture/kubernetes.html"><strong aria-hidden="true">5.3.</strong> Kubernetes Cluster</a></li></ol></li><li class="chapter-item "><li class="part-title">Docker Images</li><li class="chapter-item "><a href="docker/base-images.html"><strong aria-hidden="true">6.</strong> Base Images</a><a class="toggle"><div>❱</div></a></li><li><ol class="section"><li class="chapter-item "><a href="docker/languages/c.html"><strong aria-hidden="true">6.1.</strong> C</a></li><li class="chapter-item "><a href="docker/languages/java.html"><strong aria-hidden="true">6.2.</strong> Java</a></li><li class="chapter-item "><a href="docker/languages/javascript.html"><strong aria-hidden="true">6.3.</strong> JavaScript</a></li><li class="chapter-item "><a href="docker/languages/python.html"><strong aria-hidden="true">6.4.</strong> Python</a></li><li class="chapter-item "><a href="docker/languages/befunge.html"><strong aria-hidden="true">6.5.</strong> Befunge</a></li></ol></li><li class="chapter-item "><a href="docker/standalone-images.html"><strong aria-hidden="true">7.</strong> Standalone Images</a></li><li class="chapter-item "><a href="docker/custom-dockerfiles.html"><strong aria-hidden="true">8.</strong> Custom Dockerfiles</a></li><li class="chapter-item affix "><li class="part-title">Jenkins Configuration</li><li class="chapter-item "><a href="jenkins/jcasc.html"><strong aria-hidden="true">9.</strong> Configuration as Code (JCasC)</a></li><li class="chapter-item "><a href="jenkins/jobs.html"><strong aria-hidden="true">10.</strong> Jobs Overview</a><a class="toggle"><div>❱</div></a></li><li><ol class="section"><li class="chapter-item "><a href="jenkins/base-images-jobs.html"><strong aria-hidden="true">10.1.</strong> Base Images Jobs</a></li><li class="chapter-item "><a href="jenkins/link-project.html"><strong aria-hidden="true">10.2.</strong> Link Project Job</a></li></ol></li><li class="chapter-item "><a href="jenkins/pipeline.html"><strong aria-hidden="true">11.</strong> Pipeline</a><a class="toggle"><div>❱</div></a></li><li><ol class="section"><li class="chapter-item "><a href="jenkins/language-detection.html"><strong aria-hidden="true">11.1.</strong> Language Detection</a></li><li class="chapter-item "><a href="jenkins/build-process.html"><strong aria-hidden="true">11.2.</strong> Build Process</a></li><li class="chapter-item "><a href="jenkins/registry-push.html"><strong aria-hidden="true">11.3.</strong> Registry Push</a></li><li class="chapter-item "><a href="jenkins/k8s-deployment.html"><strong aria-hidden="true">11.4.</strong> Kubernetes Deployment</a></li></ol></li><li class="chapter-item "><li class="part-title">Kubernetes Deployment</li><li class="chapter-item "><a href="kubernetes/cluster-setup.html"><strong aria-hidden="true">12.</strong> Cluster Setup</a></li><li class="chapter-item "><a href="kubernetes/whanos-yml.html"><strong aria-hidden="true">13.</strong> whanos.yml Configuration</a></li><li class="chapter-item "><a href="kubernetes/deployments.html"><strong aria-hidden="true">14.</strong> Deployments</a></li><li class="chapter-item "><a href="kubernetes/services.html"><strong aria-hidden="true">15.</strong> Services and Networking</a></li><li class="chapter-item affix "><li class="part-title">User Guide</li><li class="chapter-item "><a href="user-guide/compatible-repo.html"><strong aria-hidden="true">16.</strong> Creating a Whanos-Compatible Repository</a></li><li class="chapter-item "><a href="user-guide/linking-project.html"><strong aria-hidden="true">17.</strong> Linking a Project</a></li><li class="chapter-item "><a href="user-guide/configuring-deployment.html"><strong aria-hidden="true">18.</strong> Configuring Deployment</a></li><li class="chapter-item "><a href="user-guide/private-repos.html"><strong aria-hidden="true">19.</strong> Private Repositories</a></li><li class="chapter-item "><a href="user-guide/monitoring.html"><strong aria-hidden="true">20.</strong> Monitoring Builds</a></li><li class="chapter-item affix "><li class="part-title">Administration</li><li class="chapter-item "><a href="admin/terraform.html"><strong aria-hidden="true">21.</strong> Deployment with Terraform</a></li><li class="chapter-item "><a href="admin/infrastructure.html"><strong aria-hidden="true">22.</strong> Infrastructure Management</a></li><li class="chapter-item "><a href="admin/backup.html"><strong aria-hidden="true">23.</strong> Backup and Recovery</a></li><li class="chapter-item "><a href="admin/scaling.html"><strong aria-hidden="true">24.</strong> Scaling</a></li><li class="chapter-item affix "><li class="part-title">Troubleshooting</li><li class="chapter-item "><a href="troubleshooting/common-issues.html"><strong aria-hidden="true">25.</strong> Common Issues</a></li><li class="chapter-item "><a href="troubleshooting/jenkins.html"><strong aria-hidden="true">26.</strong> Jenkins Problems</a></li><li class="chapter-item "><a href="troubleshooting/docker.html"><strong aria-hidden="true">27.</strong> Docker Issues</a></li><li class="chapter-item "><a href="troubleshooting/kubernetes.html"><strong aria-hidden="true">28.</strong> Kubernetes Issues</a></li><li class="chapter-item "><a href="troubleshooting/network.html"><strong aria-hidden="true">29.</strong> Network Problems</a></li><li class="chapter-item affix "><li class="part-title">Reference</li><li class="chapter-item "><a href="reference/env-vars.html"><strong aria-hidden="true">30.</strong> Environment Variables</a></li><li class="chapter-item "><a href="reference/config-files.html"><strong aria-hidden="true">31.</strong> Configuration Files</a></li><li class="chapter-item "><a href="reference/cli-commands.html"><strong aria-hidden="true">32.</strong> CLI Commands</a></li><li class="chapter-item "><a href="reference/api.html"><strong aria-hidden="true">33.</strong> API Reference</a></li><li class="chapter-item affix "><li class="part-title">Appendix</li><li class="chapter-item "><a href="appendix/faq.html"><strong aria-hidden="true">34.</strong> FAQ</a></li><li class="chapter-item "><a href="appendix/glossary.html"><strong aria-hidden="true">35.</strong> Glossary</a></li><li class="chapter-item "><a href="appendix/contributing.html"><strong aria-hidden="true">36.</strong> Contributing</a></li><li class="chapter-item "><a href="appendix/license.html"><strong aria-hidden="true">37.</strong> License</a></li></ol>';
        // Set the current, active page, and reveal it if it's hidden
        let current_page = document.location.href.toString().split("#")[0].split("?")[0];
        if (current_page.endsWith("/")) {
            current_page += "index.html";
        }
        var links = Array.prototype.slice.call(this.querySelectorAll("a"));
        var l = links.length;
        for (var i = 0; i < l; ++i) {
            var link = links[i];
            var href = link.getAttribute("href");
            if (href && !href.startsWith("#") && !/^(?:[a-z+]+:)?\/\//.test(href)) {
                link.href = path_to_root + href;
            }
            // The "index" page is supposed to alias the first chapter in the book.
            if (link.href === current_page || (i === 0 && path_to_root === "" && current_page.endsWith("/index.html"))) {
                link.classList.add("active");
                var parent = link.parentElement;
                if (parent && parent.classList.contains("chapter-item")) {
                    parent.classList.add("expanded");
                }
                while (parent) {
                    if (parent.tagName === "LI" && parent.previousElementSibling) {
                        if (parent.previousElementSibling.classList.contains("chapter-item")) {
                            parent.previousElementSibling.classList.add("expanded");
                        }
                    }
                    parent = parent.parentElement;
                }
            }
        }
        // Track and set sidebar scroll position
        this.addEventListener('click', function(e) {
            if (e.target.tagName === 'A') {
                sessionStorage.setItem('sidebar-scroll', this.scrollTop);
            }
        }, { passive: true });
        var sidebarScrollTop = sessionStorage.getItem('sidebar-scroll');
        sessionStorage.removeItem('sidebar-scroll');
        if (sidebarScrollTop) {
            // preserve sidebar scroll position when navigating via links within sidebar
            this.scrollTop = sidebarScrollTop;
        } else {
            // scroll sidebar to current active section when navigating via "next/previous chapter" buttons
            var activeSection = document.querySelector('#sidebar .active');
            if (activeSection) {
                activeSection.scrollIntoView({ block: 'center' });
            }
        }
        // Toggle buttons
        var sidebarAnchorToggles = document.querySelectorAll('#sidebar a.toggle');
        function toggleSection(ev) {
            ev.currentTarget.parentElement.classList.toggle('expanded');
        }
        Array.from(sidebarAnchorToggles).forEach(function (el) {
            el.addEventListener('click', toggleSection);
        });
    }
}
window.customElements.define("mdbook-sidebar-scrollbox", MDBookSidebarScrollbox);
