const express = require('express');
const cors = require('cors');

const app = express();
const PORT = 8080;

// Middleware
app.use(cors());
app.use(express.json());

// Sample services data
const sampleServices = [
    {
        name: "twitter",
        actions: [
            {
                name: "new_mention",
                description: "User is mentioned in a tweet"
            },
            {
                name: "new_follower",
                description: "User gains a new follower"
            }
        ],
        reactions: [
            {
                name: "tweet",
                description: "Post a tweet"
            },
            {
                name: "retweet",
                description: "Retweet a specific tweet"
            }
        ]
    },
    {
        name: "github",
        actions: [
            {
                name: "new_issue",
                description: "A new issue is created on a repository"
            },
            {
                name: "new_push",
                description: "A new push is made to a branch"
            }
        ],
        reactions: [
            {
                name: "create_issue",
                description: "Create a new issue"
            },
            {
                name: "comment_issue",
                description: "Comment on an issue"
            }
        ]
    }
];

// Routes
app.get('/hello/', (req, res) => {
    res.json({
        message: "Hello AREA! Node.js is ready for real-time!",
        framework: "Express.js + Node.js",
        features: [
            "Event-driven architecture",
            "Real-time capabilities with WebSockets",
            "Excellent for I/O intensive operations",
            "Large ecosystem (npm)"
        ],
        advantages: [
            "JavaScript everywhere (frontend/backend)",
            "Non-blocking I/O",
            "Quick prototyping"
        ]
    });
});

app.get('/about.json', (req, res) => {
    // Get client IP
    const clientIp = req.headers['x-forwarded-for'] ||
                    req.connection.remoteAddress ||
                    req.socket.remoteAddress ||
                    (req.connection.socket ? req.connection.socket.remoteAddress : null) ||
                    '127.0.0.1';

    const aboutData = {
        client: {
            host: clientIp.replace(/^.*:/, '') // Remove IPv6 prefix if present
        },
        server: {
            current_time: Math.floor(Date.now() / 1000),
            services: sampleServices
        }
    };

    res.json(aboutData);
});

app.get('/realtime-demo/', (req, res) => {
    res.json({
        message: "Node.js excels at real-time features!",
        capabilities: [
            "WebSocket support",
            "Server-Sent Events",
            "Event-driven hooks",
            "Concurrent connections"
        ],
        use_cases: [
            "Real-time AREA triggers",
            "Live dashboard updates",
            "Instant notifications",
            "Streaming data"
        ]
    });
});

// Health check
app.get('/health', (req, res) => {
    res.json({
        status: 'healthy',
        timestamp: new Date().toISOString(),
        uptime: process.uptime()
    });
});

// Start server
app.listen(PORT, () => {
    console.log(`🚀 Node.js AREA POC server running on port ${PORT}`);
    console.log(`📊 Test endpoints:`);
    console.log(`   • http://localhost:${PORT}/hello/`);
    console.log(`   • http://localhost:${PORT}/about.json`);
    console.log(`   • http://localhost:${PORT}/realtime-demo/`);
    console.log(`   • http://localhost:${PORT}/health`);
});

module.exports = app;