#!/usr/bin/env node

/**
 * PostgreSQL POC - Basic AREA operations
 */

const { Client } = require('pg');

const DB_CONFIG = {
    host: 'localhost',
    port: 5432,
    user: 'postgres',
    password: 'postgres',
    database: 'area_poc'
};

async function runPostgresqlPOC() {
    console.log('🐘 PostgreSQL POC\n');

    let client;

    try {
        client = new Client(DB_CONFIG);
        await client.connect();

        await client.query('DROP TABLE IF EXISTS areas CASCADE');
        await client.query('DROP TABLE IF EXISTS services CASCADE');
        await client.query('DROP TABLE IF EXISTS users CASCADE');
        console.log('🧹 Database cleared');

        await client.query(`
            CREATE TABLE users (
                id SERIAL PRIMARY KEY,
                email VARCHAR(255) UNIQUE NOT NULL,
                name VARCHAR(100) NOT NULL,
                created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
            )
        `);

        const userResult = await client.query(
            'INSERT INTO users (email, name) VALUES ($1, $2) RETURNING id',
            ['test@area.com', 'John Doe']
        );
        console.log(`✅ User created: ${userResult.rows[0].id}`);

        await client.query(`
            CREATE TABLE services (
                id SERIAL PRIMARY KEY,
                name VARCHAR(100) UNIQUE NOT NULL,
                display_name VARCHAR(100) NOT NULL
            )
        `);

        await client.query(`
            INSERT INTO services (name, display_name) VALUES
            ('gmail', 'Gmail'), ('github', 'GitHub')
        `);
        console.log('✅ Services created');

        await client.query(`
            CREATE TABLE areas (
                id SERIAL PRIMARY KEY,
                user_id INTEGER NOT NULL REFERENCES users(id),
                name VARCHAR(255) NOT NULL,
                action_service_id INTEGER NOT NULL REFERENCES services(id),
                reaction_service_id INTEGER NOT NULL REFERENCES services(id),
                parameters JSONB DEFAULT '{}',
                enabled BOOLEAN DEFAULT true,
                created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
            )
        `);

        const areaResult = await client.query(`
            INSERT INTO areas (user_id, name, action_service_id, reaction_service_id, parameters)
            VALUES ($1, $2, 1, 2, $3) RETURNING id
        `, [
            userResult.rows[0].id,
            'Gmail to GitHub',
            JSON.stringify({ from: 'boss@company.com' })
        ]);

        console.log(`✅ Area created: ${areaResult.rows[0].id}`);

        const userAreas = await client.query(`
            SELECT a.name as area_name,
                   s1.display_name as action_service,
                   s2.display_name as reaction_service,
                   a.parameters->>'from' as email_filter
            FROM areas a
            JOIN services s1 ON a.action_service_id = s1.id
            JOIN services s2 ON a.reaction_service_id = s2.id
            WHERE a.user_id = $1
        `, [userResult.rows[0].id]);

        console.log(`✅ Found ${userAreas.rows.length} areas for user`);
        console.log(`   - Area: ${userAreas.rows[0].area_name}`);
        console.log(`   - Filter: ${userAreas.rows[0].email_filter}`);

        console.log('\n✅ PostgreSQL POC completed! 🚀');

    } catch (error) {
        console.error('❌ Error:', error.message);
    } finally {
        if (client) await client.end();
    }
}

if (require.main === module) {
    runPostgresqlPOC().catch(console.error);
}

module.exports = { runPostgresqlPOC };