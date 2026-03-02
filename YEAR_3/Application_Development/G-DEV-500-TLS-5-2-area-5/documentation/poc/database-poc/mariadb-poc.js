#!/usr/bin/env node

/**
 * MariaDB POC - Basic AREA operations
 */

const mysql = require('mysql2/promise');

const DB_CONFIG = {
    host: 'localhost',
    user: 'root',
    password: 'root',
    database: 'area_poc'
};

async function runMariadbPOC() {
    console.log('🐬 MariaDB POC\n');

    let connection;

    try {
        connection = await mysql.createConnection(DB_CONFIG);
        console.log('✅ Connected to MariaDB');

        await connection.execute('DROP DATABASE IF EXISTS area_poc');
        await connection.execute('CREATE DATABASE area_poc');
        await connection.execute('USE area_poc');
        console.log('🧹 Database cleared');

        await connection.execute(`
            CREATE TABLE users (
                id INT AUTO_INCREMENT PRIMARY KEY,
                email VARCHAR(255) UNIQUE NOT NULL,
                name VARCHAR(100) NOT NULL,
                created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
            )
        `);

        const [userResult] = await connection.execute(
            'INSERT INTO users (email, name) VALUES (?, ?)',
            ['test@area.com', 'John Doe']
        );
        console.log(`✅ User created: ${userResult.insertId}`);

        await connection.execute(`
            CREATE TABLE services (
                id INT AUTO_INCREMENT PRIMARY KEY,
                name VARCHAR(100) UNIQUE NOT NULL,
                display_name VARCHAR(100) NOT NULL
            )
        `);

        await connection.execute(
            'INSERT INTO services (name, display_name) VALUES (?, ?), (?, ?)',
            ['gmail', 'Gmail', 'github', 'GitHub']
        );
        console.log('✅ Services created');

        // Test 3: Areas table
        await connection.execute(`
            CREATE TABLE areas (
                id INT AUTO_INCREMENT PRIMARY KEY,
                user_id INT NOT NULL,
                name VARCHAR(255) NOT NULL,
                action_service_id INT NOT NULL,
                reaction_service_id INT NOT NULL,
                enabled BOOLEAN DEFAULT true,
                created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
                FOREIGN KEY (user_id) REFERENCES users(id),
                FOREIGN KEY (action_service_id) REFERENCES services(id),
                FOREIGN KEY (reaction_service_id) REFERENCES services(id)
            )
        `);

        const [areaResult] = await connection.execute(`
            INSERT INTO areas (user_id, name, action_service_id, reaction_service_id)
            VALUES (?, ?, 1, 2)
        `, [userResult.insertId, 'Gmail to GitHub']);

        console.log(`✅ Area created: ${areaResult.insertId}`);

        const [userAreas] = await connection.execute(`
            SELECT a.name as area_name,
                   s1.display_name as action_service,
                   s2.display_name as reaction_service
            FROM areas a
            JOIN services s1 ON a.action_service_id = s1.id
            JOIN services s2 ON a.reaction_service_id = s2.id
            WHERE a.user_id = ?
        `, [userResult.insertId]);

        console.log(`✅ Found ${userAreas.length} areas for user`);
        console.log('\n✅ MariaDB POC completed! 🚀');

    } catch (error) {
        console.error('❌ Error:', error.message);
    } finally {
        if (connection) await connection.end();
    }
}

if (require.main === module) {
    runMariadbPOC().catch(console.error);
}

module.exports = { runMariadbPOC };
